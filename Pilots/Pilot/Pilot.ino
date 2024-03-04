#include <LiquidCrystal_I2C.h>
#include "Joystick.h"
#include "Encoder.h"
#include "DebounceButton.h"
#include "Ebyte32.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
Joystick joystick(PA0, PA1);
Encoder encoder(PB14, PB15);
DebounceButton upButton(PA5);
DebounceButton downButton(PA6);
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

void printConnection()
{
  lcd.setCursor(0, 0);
  if (isConnected)
  {
    lcd.print("CONNECTED       ");
  }
  else
  {
    lcd.print("DISCONNECTED    ");
  }
}

void printPowerState()
{
  char line[17];
  lcd.setCursor(9, 1);
  sprintf(line, "%3s|%3s", powerStateToString(pilotMessage.powerState), powerStateToString(droneMessage.powerState));
  lcd.print(line);
}

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
  joystick.begin();
  encoder.begin();
  upButton.begin();
  downButton.begin();
  ebyte32.begin();
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("STARTED");
  delay(500);

  printConnection();
  printPowerState();
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
      printConnection();
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
      printPowerState();
    }

    if (previousDronePowerState != droneMessage.powerState)
    {
      printPowerState();
      previousDronePowerState = droneMessage.powerState;
    }
  }
  if (micros() - lastDroneMessageTime > disconnectedThreshold)
  {
    isConnected = false;
    printConnection();
  }

  if (pilotMessage.powerState != PowerState::none)
  {
    // handle joystick input
    Vector2d joystickVector = joystick.getVector();
    pilotMessage.pitchAngle = -maxTurnAngle * joystickVector.y;
    pilotMessage.rollAngle = maxTurnAngle * joystickVector.x;

    // handle encoder input
    int yawAngle =  encoder.getModuloAngleInDegree();
    pilotMessage.yawAngle = yawAngle == 0 ? 0 : 360.0f - yawAngle;

    // handle up and down buttons input
    bool upButtonState = upButton.read();
    bool downButtonState = downButton.read();
    // both button pressed
    if (upButtonState && downButtonState)
    {
      if (!bothButtonPressed)
      {
        bothButtonPressed = true;
        firstTimeBothButtonPressed = micros();
      }
      if (!bothButtonTriggered && micros() - firstTimeBothButtonPressed > bothButtonThreshold)
      {
        bothButtonTriggered = true; 
        pilotMessage.powerState = pilotMessage.powerState == PowerState::on ? PowerState::off : PowerState::on;
        printPowerState();
      }
      pilotMessage.verticalSpeed *= speedDropFactor;
    }
    else
    {
      bothButtonPressed = false;
      bothButtonTriggered = false;

      // up button pressed
      if (upButtonState)
      {
        if (!upButtonPressed)
        {
          upButtonPressed = true;
          firstTimeUpButtonPressed = micros();
        }
        pilotMessage.verticalSpeed = min((micros() - firstTimeUpButtonPressed) / fullSpeedTime * maxDroneVerticalSpeed, maxDroneVerticalSpeed);
      }
      else
      {
        upButtonPressed = false;
      }

      // down button pressed
      if (downButtonState)
      {
        if (!downButtonPressed)
        {
          downButtonPressed = true;
          firstTimeDownButtonPressed = micros();
        }
        pilotMessage.verticalSpeed = -min((micros() - firstTimeDownButtonPressed) / fullSpeedTime * maxDroneVerticalSpeed, maxDroneVerticalSpeed);
      }
      else
      {
        downButtonPressed = false;
      }

      // none button pressed
      if (!upButtonState && !downButtonState)
      {
        pilotMessage.verticalSpeed *= speedDropFactor;
      }
    }
  }
  
  // send message to the drone
  if (nLoop % sendPeriod == 0)
  {
    ebyte32.send<PilotMessage, 1>(droneAddress, channel, pilotMessage);
  }

  ++nLoop;
  // wait for end of the cycle
  while(micros() - loopTime < loopPeriod);
  
}
