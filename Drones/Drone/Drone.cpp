#include  "Drone.h"
#include <Wire.h>

void Drone::begin()
{
  // begin motors
  motor0.begin();
  motor1.begin();
  motor2.begin();
  motor3.begin();

  // init sensors
  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  gyroscope.begin();
  accelerometer.begin();
  barometer.begin();

  // init low-pass filters
  readGyroscopeData();
  readAccelerometerData();
  readBarometerData();

  rollRate = gyroscopeData.rollRate;
  pitchRate = gyroscopeData.pitchRate;
  yawRate = gyroscopeData.yawRate;
  accelerometerX = accelerometerData.x;
  accelerometerY = accelerometerData.y;
  accelerometerZ = accelerometerData.z;
  barometerAltitude = barometerData;

  rollRateLowPassFilter.init(gyroscopeData.rollRate);
  pitchRateLowPassFilter.init(gyroscopeData.pitchRate);
  yawRateLowPassFilter.init(gyroscopeData.yawRate);
  accelerometerXLowPassFilter.init(accelerometerData.x);
  accelerometerYLowPassFilter.init(accelerometerData.y);
  accelerometerZLowPassFilter.init(accelerometerData.z);
  barometerLowPassFilter.init(barometerData);

  // ground level calculation
  calculateDroneGroundLevel();

  // init remote communication
  ebyte32.begin();
  delay(2750);
}

void Drone::step()
{
  stepStartTime = micros();
  handleSensors();
  handleRemoteControl();
  handleStateChanges();
  setMotorsSpeed();
  waitTillEndOfStep();
}

void Drone::readGyroscopeData()
{
  gyroscopeData = gyroscope.read(); 
  gyroscopeData.pitchRate = -gyroscopeData.pitchRate; 
  gyroscopeData.yawRate = -gyroscopeData.yawRate;
}

void Drone::readAccelerometerData()
{
  accelerometerData = accelerometer.read(); 
  accelerometerData.y = -accelerometerData.y; 
  accelerometerData.z = -accelerometerData.z;
}

void Drone::readBarometerData()
{
  barometerData = barometer.readAltitude();
}

void Drone::calculateDroneGroundLevel()
{
  float sum = 0.0f;
  for (int i = 0; i < N_GROUND_LEVEL_CALCULATING_MEASUREMENTS; ++i)
  {
    readBarometerData();
    barometerAltitude = barometerLowPassFilter.step(barometerData);
    sum += barometerAltitude;
    delay(1);
  }
  groundLevelAltitude = sum / N_GROUND_LEVEL_CALCULATING_MEASUREMENTS;
}

void Drone::handleSensors()
{
  // read sensors
  readGyroscopeData();
  readAccelerometerData();
  readBarometerData();

  // low-pass filtering
  rollRate = rollRateLowPassFilter.step(gyroscopeData.rollRate);
  pitchRate = pitchRateLowPassFilter.step(gyroscopeData.pitchRate);
  yawRate = yawRateLowPassFilter.step(gyroscopeData.yawRate);
  accelerometerX = accelerometerXLowPassFilter.step(accelerometerData.x);
  accelerometerY = accelerometerYLowPassFilter.step(accelerometerData.y);
  accelerometerZ = accelerometerZLowPassFilter.step(accelerometerData.z);
  barometerAltitude = barometerLowPassFilter.step(barometerData);

  // position and movment calculations
  calculateRollAndPitchAngles();
  calculateAltitudeAndVerticalMovment();
}

void Drone::calculateRollAndPitchAngles()
{
  const float rollAngleMeasure = toDegrees(atan(accelerometerY / sqrt(accelerometerX * accelerometerX + accelerometerZ * accelerometerZ)));
  const float pitchAngleMeasure = toDegrees(-atan(accelerometerX / sqrt(accelerometerY * accelerometerY + accelerometerZ * accelerometerZ)));

  rollAngle = rollAngleKalmanFilter.update(rollRate, rollAngleMeasure);
  pitchAngle = pitchAngleKalmanFilter.update(pitchRate, pitchAngleMeasure);
}

void Drone::calculateAltitudeAndVerticalMovment()
{
  float rollAnglleRadians = toRadians(rollAngle);
  float pitchAngleRadians = toRadians(pitchAngle);
  verticalAccelertion = (-sin(pitchAngleRadians) * accelerometerX + cos(pitchAngleRadians) * sin(rollAnglleRadians) * accelerometerY + cos(pitchAngleRadians) * cos(rollAnglleRadians) * accelerometerZ - 1) * 9.81 * 100;

  float measuredAltitude = barometerAltitude - groundLevelAltitude;
  BLA::Matrix<2, 1> heightAndVelocity = heightAndVelocityKalmanFilter.update(verticalAccelertion, measuredAltitude);
  altitude = heightAndVelocity(0, 0);
  verticalSpeed = heightAndVelocity(0, 1);
}

void Drone::handleRemoteControl()
{
  if (ebyte32.avalibleToReceive<PilotMessage, 1>())
  {
    if (ebyte32.receive<PilotMessage, 1>(pilotMessage))
    {
      lastPilotMessageTime = micros();
      isConnected = true;
      if (pilotMessage.powerState != PowerState::none)
      {
        droneMessage.powerState = pilotMessage.powerState;
      }
      ebyte32.send(PILOT_ADDRESS, CHANNEL, droneMessage);
    }
  }
  if (isConnected && micros() - lastPilotMessageTime > DISCONNECT_THRESHOLD)
  {
    isConnected = false;
    disconnectTime = micros();
  }
}

void Drone::handleStateChanges()
{
  switch (state)
  {
    case State::off:
    {
      if (isConnected && droneMessage.powerState == PowerState::on)
      {
        state = State::userControlled;
      }
      break;
    }
    case State::userControlled:
    {
      if (isConnected && droneMessage.powerState == PowerState::off)
      {
        state = State::off;
        droneMessage.powerState = PowerState::off;
      }
      if (!isConnected)
      {
        if (micros() - disconnectTime <= LONG_DISCONNECT_THRESHOLD)
        {
          state = State::disconnected;
        }
        else
        {
          state = State::longTimeDisconnected;
          resetOnTheGroundTest();
        }
      }
      break;
    }
    case State::disconnected:
    {
      if (isConnected)
      {
        if (droneMessage.powerState == PowerState::off)
        {
          state = State::off;
          droneMessage.powerState = PowerState::off;
        }
        else
        {
          state = State::userControlled;
        }
      }
      else if (micros() - disconnectTime > LONG_DISCONNECT_THRESHOLD)
      {
        state = State::longTimeDisconnected;
        resetOnTheGroundTest();
      }
      break;
    }
    case State::longTimeDisconnected:
    {
      if (isConnected)
      {
        if (droneMessage.powerState == PowerState::off)
        {
          state = State::off;
          droneMessage.powerState = PowerState::off;
        }
        else
        {
          state = State::userControlled;
        }
      }
      else if (isOnTheGround())
      {
        state = State::off;
        droneMessage.powerState = PowerState::off;
      }
      break;
    }
  }

  if (isUpsideDown())
  {
    state = State::off;
    droneMessage.powerState = PowerState::off;
  }
}

bool Drone::isOnTheGround()
{
  if (targetVerticalSpeed <= -10 && abs(verticalSpeed) < NOT_MOVING_THRESHOLD)
  {
    ++nNotMovingSteps;
  }
  else
  {
    nNotMovingSteps = max(0, nNotMovingSteps - 9);
  }
  return nNotMovingSteps >= MINIMUM_NOT_MOVING_STEPS;
}

void Drone::resetOnTheGroundTest()
{
  nNotMovingSteps = 0;
}

bool Drone::isUpsideDown()
{
  return accelerometerZ <= UPSIDE_DOWN_THRESHOLD;
}

void Drone::setMotorsSpeed()
{
  switch (state)
  {
    case State::userControlled:
    {
      targetRollAngle = pilotMessage.rollAngle;
      targetPitchAngle = pilotMessage.pitchAngle;
      targetYawRate = pilotMessage.yawRate;
      targetVerticalSpeed = pilotMessage.verticalSpeed;
      targetRollRate = max(-30, min(targetRollAngle - rollAngle, 30));
      targetPitchRate = max(-30, min(targetPitchAngle - pitchAngle, 30));
      break;
    }
    case State::disconnected:
    {
      targetRollAngle = 0.0f;
      targetPitchAngle = 0.0f;
      targetYawRate = 0.0f;
      targetVerticalSpeed = 0.0f;
      targetRollRate = max(-30, min(targetRollAngle - rollAngle, 30));
      targetPitchRate = max(-30, min(targetPitchAngle - pitchAngle, 30));
      break;
    }
    case State::longTimeDisconnected:
    {
      targetRollAngle = 0.0f;
      targetPitchAngle = 0.0f;
      targetYawRate = 0.0f;
      targetVerticalSpeed = LANDING_SPEED;
      targetRollRate = max(-30, min(targetRollAngle - rollAngle, 30));
      targetPitchRate = max(-30, min(targetPitchAngle - pitchAngle, 30));
      break;
    }
  }

  if (state == State::off)
  {
    motor0Speed = motor1Speed = motor2Speed = motor3Speed = 0.0f;
    resetPID();
  }
  else
  {
    float inputRoll = rollRatePID.step(targetRollRate - rollRate);
    float inputPitch = pitchRatePID.step(targetPitchRate - pitchRate);
    float inputYaw = yawRatePID.step(targetYawRate - yawRate);
    float inputThrottle = max(0.2, min(verticalAccelertionPID.step(targetVerticalSpeed - verticalSpeed) + NORMAL_MOTOR_SPEED, 0.8));

    motor0Speed = inputThrottle - inputPitch - inputRoll - inputYaw;
    motor1Speed = inputThrottle + inputPitch - inputRoll + inputYaw;
    motor2Speed = inputThrottle + inputPitch + inputRoll - inputYaw;
    motor3Speed = inputThrottle - inputPitch + inputRoll + inputYaw;

    motor0Speed = linearizeThrottle(motor0Speed);
    motor1Speed = linearizeThrottle(motor1Speed);
    motor2Speed = linearizeThrottle(motor2Speed);
    motor3Speed = linearizeThrottle(motor3Speed);
  }

  motor0.setSpeed(motor0Speed);
  motor1.setSpeed(motor1Speed);
  motor2.setSpeed(motor2Speed);
  motor3.setSpeed(motor3Speed);
}

void Drone::waitTillEndOfStep()
{
  while (micros() - stepStartTime < STEP_TIME);
}

void Drone::resetPID()
{
  rollRatePID.reset();
  pitchRatePID.reset();
  yawRatePID.reset();
  verticalAccelertionPID.reset();
}

float Drone::linearizeThrottle(double throttle)
{
  double value = throttle < 0.01 ? 0.0 : 0.0247 + (1.9776 + (-2.736128 + (2.59260416 - 0.8589934592 * throttle) * throttle) * throttle) * throttle;
  return max(0.0, min(value, 1.0));
}


float Drone::toRadians(float angle)
{
  return angle * 3.14159265359f / 180.0f;
}

float Drone::toDegrees(float angle)
{
  return angle * 180.0 / 3.14159265359f;
}