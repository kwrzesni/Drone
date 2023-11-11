#include <Wire.h>
#include "Ebyte32.h"
#include "Motor.h"
#include "Gyroscope.h"
#include "Accelerometer.h"
#include "BMP388.h"
#include "Magnetometer.h"
#include "Calculator.h"
#include "PID.h"

//#define TEST
//#define PRINT_POSTION
//#define PRINT_TIME
//#define PRINT_PILOT_MESSAGE
//#define PRINT_PID_OUTPUT
#define PRINT defined(PRINT_POSTION) ||  defined(PRINT_TIME) || defined(PRINT_PILOT_MESSAGE) || defined(PRINT_PID_OUTPUT)

enum class PowerState : unsigned char
{
  on,
  off,
  none
};

struct PilotMessage
{
  PowerState powerState;
  float pitchAngle;
  float rollAngle;
  float yawAngle;
  float verticalSpeed;
};

struct DroneMessage
{
  PowerState powerState;
};

const long disconnectedThreshold = 1000000; // [μs]
const long longDisconnectedThreshold = 10000000; // [μs]
const long loopPeriod = 6000; // [μs]
bool isConnected;
bool longTimeDisconnected = false;
long lastPilotMessageTime = -999999;
PilotMessage pilotMessage;
DroneMessage droneMessage = {PowerState::off};
unsigned short pilotAddress = 0x2137;
unsigned char channel = 5;
Ebyte32 ebyte32(&Serial3, PB12);
Motor motor0(PA3);
Motor motor1(PA0);
Motor motor2(PA1);
Motor motor3(PA2);
Gyroscope gyroscope;
Accelerometer accelerometer;
BMP388 barometer;
Magnetometer magnetometer;
Calculator calculator(loopPeriod / 1000000.0);
//PID rollRatePID{loopPeriod / 1000000.0, 0.000596324495246172f, 0.000217423864781117f, 0.0000413571213203448f};
//PID pitchRatePID{loopPeriod / 1000000.0, 0.000670442670927328f, 0.000256029232402718f, 0.0000447767486366348f};
PID rollRatePID{loopPeriod / 1000000.0, 0.000596324495246172f, 0.000217423864781117f, 0.0f};
PID pitchRatePID{loopPeriod / 1000000.0, 1.5 * 0.000670442670927328f, 0.000256029232402718f, 0.0f};
PID yawRatePID{loopPeriod / 1000000.0, 0.0003786872424349f, 0.0000144901288601733f, 0.00219841844854243f};
//PID verticalSpeedPID{loopPeriod / 1000000.0, 20 * 0.01003002376891822f, 0.00256626346190803f, 0.0000f};
PID verticalAccelertionPID{loopPeriod / 1000000.0, 0.01003002376891822f, 0.00256626346190803f, 0.0000f};
float motor0Speed = 0.0f;
float motor1Speed = 0.0f;
float motor2Speed = 0.0f;
float motor3Speed = 0.0f;
float desiredRollAngle = 0.0f;
float desiredRollRate = 0.0;
float desiredPitchAngle = 0.0f;
float desiredPitchRate = 0.0;
float desiredYawAngle = 0.0f;
float desiredYawRate = 0.0;
float desiredVerticalSpeed = 0.0f;
float desiredVerticalAccelertion = 0.0f;
float inputThrottle = 0.0f;
float inputRoll = 0.0f;
float inputPitch = 0.0f;
float inputYaw = 0.0f;
long startTime = 0;

const char* powerStateToString(PowerState powerState)
{
  switch (powerState)
  {
  case PowerState::on:
    return "ON";
  case PowerState::off:
    return "OFF";
  case PowerState::none:
    return "-";
  }
}

bool isUpsiteDown(float accZ)
{
  return accZ <= -0.75;
}

void setup() 
{
  motor0.begin();
  motor1.begin();
  motor2.begin();
  motor3.begin();

  #if PRINT
  Serial.begin(9600);
  #endif

  Wire.setClock(400000);
  Wire.begin();
  delay(250);
  
  gyroscope.begin();
  accelerometer.begin();
  barometer.begin();
  magnetometer.begin();
  calculator.initBarometerGroundLevelAltitude(barometer);
  ebyte32.begin();
  delay(2750);

  startTime = millis();
}

void loop() 
{
  long loopTime = micros();

  //position
  Gyroscope::Data gyroscopeData = gyroscope.read();
  Accelerometer::Data accelerometerData = accelerometer.read();
  Magnetometer::Data magnetometerData = magnetometer.read();
  float altitude = barometer.readAltitude();
  Calculator::AngleData angles = calculator.calculateAngles(gyroscopeData, accelerometerData, magnetometerData);
  float verticalAccelertion = calculator.calculateVerticalAccelertion(accelerometerData);
  Calculator::HeightAndVelocityData heightAndVelocity = calculator.calculateHeightAndVelocity(altitude, verticalAccelertion);

  // handle drone message
  if (ebyte32.avalibleToReceive<PilotMessage, 1>())
  {
    if (ebyte32.receive<PilotMessage, 1>(pilotMessage))
    {
      lastPilotMessageTime = loopTime;
      isConnected = true;
      if (pilotMessage.powerState != PowerState::none)
      {
        if (droneMessage.powerState == PowerState::off && pilotMessage.powerState == PowerState::on)
        {
          calculator.setZeroYawAngle(angles.yawAngle);
        }
        droneMessage.powerState = pilotMessage.powerState;
      }
      else
      {
        calculator.setZeroYawAngle(angles.yawAngle);
      }
      ebyte32.send(pilotAddress, channel, droneMessage);

      desiredRollAngle = pilotMessage.rollAngle;
      desiredPitchAngle = pilotMessage.pitchAngle;
      desiredYawAngle = pilotMessage.yawAngle;
      desiredVerticalSpeed = pilotMessage.verticalSpeed;

      #ifdef PRINT_PILOT_MESSAGE
      Serial.print("pilotMessage.powerState: "); Serial.println(powerStateToString(pilotMessage.powerState));
      Serial.print("pilotMessage.rollAngle: "); Serial.println(pilotMessage.rollAngle); 
      Serial.print("pilotMessage.pitchAngle: "); Serial.println(pilotMessage.pitchAngle); 
      Serial.print("pilotMessage.yawAngle: "); Serial.println(pilotMessage.yawAngle); 
      Serial.print("pilotMessage.verticalSpeed: "); Serial.println(pilotMessage.verticalSpeed);
      #endif
    }
  }
  if (micros() - lastPilotMessageTime > disconnectedThreshold)
  {
    isConnected = false;
  }

  if (isUpsiteDown(accelerometerData.z))
  {
    motor0Speed = motor1Speed = motor2Speed = motor3Speed = 0.0f;
    rollRatePID.reset();
    pitchRatePID.reset();
    yawRatePID.reset();
    verticalAccelertionPID.reset();
  }
  else if (droneMessage.powerState == PowerState::on)
  {
    #ifndef TEST
    if (!isConnected)
    {
      if (micros() - lastPilotMessageTime <= longDisconnectedThreshold)
      {
        longTimeDisconnected = false;
      }
      else if (!longTimeDisconnected)
      {
        longTimeDisconnected = true;
        calculator.resetOnTheGroundTest();
      }
      
      if (!longTimeDisconnected)
      {
        desiredRollAngle = 0.0f;
        desiredPitchAngle = 0.0f;
        desiredYawAngle = 0.0f;
        desiredVerticalSpeed = 0.0f;
      }
      else
      {
        desiredRollAngle = 0.0f;
        desiredPitchAngle = 0.0f;
        desiredYawAngle = 0.0f;
        desiredVerticalSpeed = -10.0f;
        if (calculator.isOnTheGround(heightAndVelocity.velocity))
        {
          droneMessage.powerState = PowerState::off;
        }
      }
    }

    desiredVerticalAccelertion = desiredVerticalSpeed - heightAndVelocity.velocity;
    inputThrottle = min(0.5 + verticalAccelertionPID.step(desiredVerticalAccelertion - verticalAccelertion), 0.85);
    desiredRollRate = desiredRollAngle - angles.rollAngle;
    inputRoll = rollRatePID.step(desiredRollRate - gyroscopeData.rollRate);
    desiredPitchRate = desiredPitchAngle - angles.pitchAngle;
    inputPitch = pitchRatePID.step(desiredPitchRate - gyroscopeData.pitchRate);
    desiredYawRate = calculator.getAngleDifference(desiredYawAngle, angles.yawAngle);
    inputYaw = 0;//yawRatePID.step(desiredYawRate - gyroscopeData.yawRate);

    #ifdef PRINT_PID_OUTPUT
    Serial.print("inputThrottle: "); Serial.println(inputThrottle); 
    Serial.print("inputRoll: "); Serial.println(inputRoll); 
    Serial.print("inputPitch: "); Serial.println(inputPitch); 
    Serial.print("inputYaw: "); Serial.println(inputYaw); 
    #endif

    motor0Speed = inputThrottle - inputPitch - inputRoll - inputYaw + 0.05;
    motor1Speed = inputThrottle + inputPitch - inputRoll + inputYaw;
    motor2Speed = inputThrottle + inputPitch + inputRoll - inputYaw;
    motor3Speed = inputThrottle - inputPitch + inputRoll + inputYaw + 0.05;

    motor0Speed = min(motor0Speed, 1.0f);
    motor1Speed = min(motor1Speed, 1.0f);
    motor2Speed = min(motor2Speed, 1.0f);
    motor3Speed = min(motor3Speed, 1.0f);

    if (motor0Speed < 0.04)
    {
      motor0Speed = 0.0f;
    }
    if (motor1Speed < 0.04)
    {
      motor1Speed = 0.0f;
    }
    if (motor2Speed < 0.04)
    {
      motor2Speed = 0.0f;
    }
    if (motor3Speed < 0.04)
    {
      motor3Speed = 0.0f;
    }
    
    #endif

    //test
    #ifdef TEST
    if (isConnected)
    {
      if (pilotMessage.pitchAngle >= 29.0f)
      {
        motor0Speed = motor1Speed = motor2Speed = motor3Speed = 0.1f;
      }
      else
      {
        motor0Speed = motor1Speed = motor2Speed = motor3Speed = 0.0f;
      }
    }
    #endif
  }
  else
  {
    motor0Speed = motor1Speed = motor2Speed = motor3Speed = 0.0f;
    rollRatePID.reset();
    pitchRatePID.reset();
    yawRatePID.reset();
    verticalAccelertionPID.reset();
  }

  motor0.setSpeed(motor0Speed);
  motor1.setSpeed(motor1Speed);
  motor2.setSpeed(motor2Speed);
  motor3.setSpeed(motor3Speed);

  #ifdef PRINT_POSTION
  Serial.print(angles.rollAngle); Serial.print(" "); Serial.print(angles.pitchAngle); Serial.print(" "); Serial.print(angles.yawAngle); Serial.print(" "); Serial.print(heightAndVelocity.height); Serial.print(" "); Serial.print(heightAndVelocity.velocity); Serial.println(" ");
  #endif


  // wait for end of the cycle
  #ifdef PRINT_TIME
  Serial.print("calculation end: "); Serial.print(micros() - loopTime);
  #endif
  while(micros() - loopTime < loopPeriod);
  #ifdef PRINT_TIME
  Serial.print(", loop end: "); Serial.println(micros() - loopTime);
  #endif
}