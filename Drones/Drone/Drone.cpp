#include  "Drone.h"
#include <Wire.h>

void Drone::begin()
{
  // begin motors
  setMotorTimers();

  // init sensors
  Wire.begin();
  Wire.setClock(1000000);
  delay(250);
  gyroscope.begin();
  accelerometer.begin();
  barometer.begin();
  readGyroscopeData();
  readAccelerometerData();
  readBarometerData();
  delay(100);
  gyroscope.init();

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

void Drone::setMotorTimers()
{
  TIMER2_BASE->CR1 = TIMER_CR1_CEN | TIMER_CR1_ARPE;
  TIMER2_BASE->CR2 = 0;
  TIMER2_BASE->SMCR = 0;
  TIMER2_BASE->DIER = 0;
  TIMER2_BASE->EGR = 0;
  TIMER2_BASE->CCMR1 = (0b110 << 4) | TIMER_CCMR1_OC1PE |(0b110 << 12) | TIMER_CCMR1_OC2PE;
  TIMER2_BASE->CCMR2 = (0b110 << 4) | TIMER_CCMR2_OC3PE |(0b110 << 12) | TIMER_CCMR2_OC4PE;
  TIMER2_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E | TIMER_CCER_CC3E | TIMER_CCER_CC4E;
  TIMER2_BASE->PSC = 71;
  TIMER2_BASE->DCR = 0;

  TIMER2_BASE->CCR1 = 1000;
  TIMER2_BASE->CCR2 = 1000;
  TIMER2_BASE->CCR3 = 1000;
  TIMER2_BASE->CCR4 = 1000;
  pinMode(PA0, PWM);
  pinMode(PA1, PWM);
  pinMode(PA2, PWM);
  pinMode(PA3, PWM);
  TIMER2_BASE->ARR = 5000;
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
    delay(20);
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
      targetRollRate = rollAnglePID.step(targetRollAngle - rollAngle);
      targetPitchRate = pitchAnglePID.step(targetPitchAngle - pitchAngle);
      break;
    }
    case State::disconnected:
    {
      targetRollAngle = 0.0f;
      targetPitchAngle = 0.0f;
      targetYawRate = 0.0f;
      targetVerticalSpeed = 0.0f;
      targetRollRate = rollAnglePID.step(0.0f - rollAngle);
      targetPitchRate = pitchAnglePID.step(0.0f - pitchAngle);
      break;
    }
    case State::longTimeDisconnected:
    {
      targetRollAngle = 0.0f;
      targetPitchAngle = 0.0f;
      targetYawRate = 0.0f;
      targetVerticalSpeed = LANDING_SPEED;
      targetRollRate = rollAnglePID.step(0.0f - rollAngle);
      targetPitchRate = pitchAnglePID.step(0.0f - pitchAngle);
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
    float inputThrottle = 800.0f * pilotMessage.verticalSpeed;

    motor0Speed = clamp(inputThrottle - inputPitch - inputRoll - inputYaw, MINIMUM_MOTOR_SPEED_TO_SPIN, 1000.0f);
    motor1Speed = clamp(inputThrottle + inputPitch - inputRoll + inputYaw, MINIMUM_MOTOR_SPEED_TO_SPIN, 1000.0f);
    motor2Speed = clamp(inputThrottle + inputPitch + inputRoll - inputYaw, MINIMUM_MOTOR_SPEED_TO_SPIN, 1000.0f);
    motor3Speed = clamp(inputThrottle - inputPitch + inputRoll + inputYaw, MINIMUM_MOTOR_SPEED_TO_SPIN, 1000.0f);
  }

  setMotorsPWM();
}

void Drone::setMotorsPWM()
{
  TIMER2_BASE->CCR1 = 1000 + motor1Speed;
  TIMER2_BASE->CCR2 = 1000 + motor2Speed;
  TIMER2_BASE->CCR3 = 1000 + motor0Speed;
  TIMER2_BASE->CCR4 = 1000 + motor3Speed;
  TIMER2_BASE->CNT = 5000;
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

float Drone::toRadians(float angle)
{
  return angle * 3.14159265359f / 180.0f;
}

float Drone::toDegrees(float angle)
{
  return angle * 180.0 / 3.14159265359f;
}

float Drone::clamp(float value, float min, float max)
{
  if (value < min)
  {
    return min;
  }
  if (value > max)
  {
    return max;
  }
  return value;
}