#include <Ebyte32.h>

Ebyte32 ebyte32(&Serial3, PB12);

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


const long bothButtonThreshold = 3000000; //[μs]
const long disconnectedThreshold = 2000000; //[μs]
const long loopPeriod = 4000; //[μs]
const float maxDroneVerticalSpeed = 150; //[cm/s]
const float fullSpeedTime = 5000000; //[μs]
const float speedDropFactor = 0.97f;
const float maxTurnAngle = 30; //[degree]
const int sendPeriod = 32; // [loops]

bool isConnected = false;
long lastDroneMessageTime = -999999;
PilotMessage pilotMessage = {PowerState::none, 0, 0, 0, 0};
DroneMessage droneMessage = {PowerState::none};
unsigned short droneAddress = 0x6969;
unsigned char channel = 5;
bool bothButtonPressed = false;
long firstTimeBothButtonPressed = 0;
bool bothButtonTriggered = false;
bool upButtonPressed = false;
long firstTimeUpButtonPressed = 0;
bool downButtonPressed = false;
long firstTimeDownButtonPressed = 0;
PowerState previousDronePowerState = PowerState::none;
int nLoop = 0;

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

void setup() 
{
  Serial.begin(9600);
  ebyte32.begin();
  delay(500);
}

void loop() 
{
  long loopTime = micros();

  // handle drone message
  if (ebyte32.avalibleToReceive<DroneMessage>())
  {
    if (!isConnected)
    {
      isConnected = true;
    }
    ebyte32.receive(droneMessage);
    lastDroneMessageTime = loopTime;
    
    if (pilotMessage.powerState == PowerState::none)
    {
      if (droneMessage.powerState == PowerState::none)
      {
        pilotMessage.powerState = droneMessage.powerState = PowerState::off;
      }
      else
      {
        pilotMessage.powerState = droneMessage.powerState;
      }
      previousDronePowerState = droneMessage.powerState;
    }

    if (previousDronePowerState != droneMessage.powerState)
    {
      previousDronePowerState = droneMessage.powerState;
    }
  }
  if (micros() - lastDroneMessageTime > disconnectedThreshold)
  {
    isConnected = false;
  }

  if (Serial.available())
  {
    String command = Serial.readString();
    switch(toLowerCase(command[0]))
    {
      case 's':
      {
        pilotMessage.powerState = pilotMessage.powerState == PowerState::off ? PowerState::on : PowerState::off;
        break;
      }
      case 'c':
      {
        pilotMessage.pitchAngle = command.substring(1).toFloat();
        break;
      }
      case 'r':
      {
        pilotMessage.rollAngle = command.substring(1).toFloat();
        break;
      }
      case 'y':
      {
        pilotMessage.yawAngle = command.substring(1).toFloat();
        break;
      }
      case 'v':
      {
        pilotMessage.verticalSpeed = command.substring(1).toFloat();
        break;
      }
    }
  }
  
  // send message to the drone
  if (nLoop % sendPeriod == 0)
  {
    Serial.print("connected: "); Serial.println(isConnected);
    Serial.print("power (S): "); Serial.print(powerStateToString(pilotMessage.powerState)); Serial.print("/"); Serial.println(powerStateToString(droneMessage.powerState));
    Serial.print("pitchAngle (C value): "); Serial.println(pilotMessage.pitchAngle, 10);
    Serial.print("rollAngle (R value): "); Serial.println(pilotMessage.rollAngle, 10);
    Serial.print("yawAngle (Y value): "); Serial.println(pilotMessage.yawAngle, 10);
    Serial.print("verticalSpeed (V value): "); Serial.println(pilotMessage.verticalSpeed, 10);
    ebyte32.send<PilotMessage, 1>(droneAddress, channel, pilotMessage);
  }

  ++nLoop;
  // wait for end of the cycle
  while(micros() - loopTime < loopPeriod);
  
}
