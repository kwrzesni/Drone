#include <AnalogDiode.h>
#include <Ebyte32.h>
#include <MultipleSevenSegmentDisplaysWithRegisters.h>
#include <Gimbal1Axis.h>
#include <EncoderWithButton.h>
#include <JoystickWithButton.h>
#include <MAX17048.h>
#include <CommonDataTypes.h>

// periphery
AnalogDiode connectionStateDiode(PA0);
AnalogDiode isDroneOnFromPilotPOVDiode(PA1);
AnalogDiode isDroneOnFromDronePOVDiode(PA2);
Ebyte32 ebyte32(&Serial3, PB12);
MultipleSevenSegmentDisplaysWithRegisters<3> batteryLevelDisplaly(PB1, PB0, PA7);
Gimbal1Axis gimbal(PA3, 350, 3650);
EncoderWithButton encoderWithButton(PB4, PB5, 40, PB3);
JoystickWithButton joystickWithButton(PA5, PA4, 2057, 2062, PA15);
MAX17048  batteryFuelGauge;

// constants
const long POWER_BUTTON_THRESHOLD = 300000; //[μs]
const long DISCONNECTED_THRESHOLD = 1000000; //[μs]
const long LOOP_PERIOD = 4000; //[μs]
const float MAX_TURN_ANGLE = 20; //[degree]
const int SEND_PERIOD = 16; // [loops]
const unsigned short DRONE_ADDRESS = 0x6969;
const unsigned char CHANNEL = 5;
const float NONE_STATE_VALUE = 0.005;
const float MAX_YAW_RATE = 10.0f;

// state variables
bool isConnected = false;
long lastDroneMessageTime = -999999;
PilotMessage pilotMessage = {PowerState::none, 0, 0, 0, 0};
DroneMessage droneMessage = {PowerState::none};
bool powerButtonPressed = false;
long firstTimePowerButtonPressed = 0;
bool bothButtonTriggered = false;
int nYawRateMessages = 0;
float previousYawRate = 0;
float previousEncoderAngle = 0;
int nLoop = 0;

void setup() 
{
  afio_cfg_debug_ports(AFIO_DEBUG_SW_ONLY);
  Wire.begin();
  connectionStateDiode.begin();
  isDroneOnFromPilotPOVDiode.begin();
  isDroneOnFromDronePOVDiode.begin();
  pinMode(PB13, OUTPUT); digitalWrite(PB13, 0);
  pinMode(PB14, OUTPUT); digitalWrite(PB14, 0);
  ebyte32.begin();
  
  batteryLevelDisplaly.begin();
  gimbal.begin();
  encoderWithButton.begin();
  joystickWithButton.begin();
  batteryFuelGauge.attatch(Wire);
}

void loop() 
{
  long loopTime = micros();

  // handle drone message
  if (ebyte32.avalibleToReceive<DroneMessage>())
  {
    ebyte32.receive<DroneMessage>(droneMessage);
    if (!isConnected)
    {
      isConnected = true;
    }
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
    }
  }
  if (micros() - lastDroneMessageTime > DISCONNECTED_THRESHOLD)
  {
    isConnected = false;
  }

  //handle inputs
  if (pilotMessage.powerState != PowerState::none)
  {
    //handle gimbal input
    pilotMessage.verticalSpeed = gimbal.read();

    // handle encoderWithButton input
    float currentEncoderAngle = encoderWithButton.getAngleInDegree();
    float encoderAngleDiff = currentEncoderAngle - previousEncoderAngle;
    previousEncoderAngle = currentEncoderAngle;
    if (pilotMessage.yawRate == 0.0f)
    {
      if (encoderAngleDiff > 0)
      {
        pilotMessage.yawRate = MAX_YAW_RATE;
        nYawRateMessages = encoderAngleDiff / 9;
      }
      else if (encoderAngleDiff < 0)
      {
        pilotMessage.yawRate = -MAX_YAW_RATE;
        nYawRateMessages = -encoderAngleDiff / 9;
      }
    }
    else
    {
      if (encoderAngleDiff * pilotMessage.yawRate > 0)
      {
        nYawRateMessages += abs(encoderAngleDiff) / 9;
      }
      else
      {
        int temp = abs(encoderAngleDiff) / 9;
        if (temp > nYawRateMessages)
        {
          pilotMessage.yawRate *= -1.0f;
          nYawRateMessages = temp - nYawRateMessages;
        }
        else
        {
          nYawRateMessages -= temp;
        }
      }
    }

    // handle joystickWithButton input
    auto joystickVector = joystickWithButton.getVector();
    pilotMessage.pitchAngle = -MAX_TURN_ANGLE * joystickVector.x;
    pilotMessage.rollAngle = -MAX_TURN_ANGLE * joystickVector.y;

    // handle buttons input
    if (encoderWithButton.readButtonState())
    {
      if (!powerButtonPressed)
      {
        powerButtonPressed = true;
        firstTimePowerButtonPressed = micros();
      }
      if (!bothButtonTriggered && micros() - firstTimePowerButtonPressed > POWER_BUTTON_THRESHOLD)
      {
        bothButtonTriggered = true; 
        pilotMessage.powerState = pilotMessage.powerState == PowerState::on ? PowerState::off : PowerState::on;
      }
    }
    else
    {
      powerButtonPressed = false;
      bothButtonTriggered = false;
    }
  }

  // display state
  connectionStateDiode.setValue(isConnected);
  if (pilotMessage.powerState == PowerState::none)
  {
    isDroneOnFromPilotPOVDiode.setValue(NONE_STATE_VALUE);
    isDroneOnFromDronePOVDiode.setValue(NONE_STATE_VALUE);
  }
  else
  {
    isDroneOnFromPilotPOVDiode.setValue(static_cast<float>(pilotMessage.powerState));
    isDroneOnFromDronePOVDiode.setValue(static_cast<float>(droneMessage.powerState));
  }
  batteryLevelDisplaly.setValue(batteryFuelGauge.accuratePercent());
  // send message to the drone
  if (nLoop % SEND_PERIOD == 0)
  {
    ebyte32.send<PilotMessage, 1>(DRONE_ADDRESS, CHANNEL, pilotMessage);
    nYawRateMessages = nYawRateMessages > 0 ? nYawRateMessages - 1 : 0;
    if (nYawRateMessages == 0)
    {
      pilotMessage.yawRate = 0.0f;
    }
  }

  ++nLoop;
  // wait for end of the cycle
  while(micros() - loopTime < LOOP_PERIOD);
}
