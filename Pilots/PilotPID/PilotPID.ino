#include <Ebyte32.h>
#include <CommonDataTypes.h>

Ebyte32 ebyte32(&Serial3, PB12);


const long bothButtonThreshold = 3000000; //[μs]
const long disconnectedThreshold = 2000000; //[μs]
const long loopPeriod = 4000; //[μs]
const float maxDroneVerticalSpeed = 150; //[cm/s]
const float fullSpeedTime = 5000000; //[μs]
const float speedDropFactor = 0.97f;
const float maxTurnAngle = 30; //[degree]
const int sendPeriod = 64; // [loops]
const int slowSendPeriod = 256;

bool isConnected = false;
long lastDroneMessageTime = -999999;
PidPilotMessage pidPilotMessage = {PowerState::off, 0, 0, 0, 0, PIDtype::pitch, 0, 0, 0};
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
unsigned long stateChangeTimepoint = 0;
unsigned long stateChangeTimepoint2 = 0;

// PIDs
float pitchP = 0.0015;
float pitchI = 0.0012;
float pitchD = 0.07;
float rollP = 0.0015;
float rollI = 0.001;
float rollD = 0.06;
float yawP = 0;
float yawI = 0;
float yawD = 0;
float speedP = 0;
float speedI = 0;
float speedD = 0;
float rollCompensation = 0.104;
float pitchCompensation = 0.22;

const char* pidStateToString(PIDtype pid)
{
  switch (pid)
  {
  case PIDtype::pitch:
    return "pitch";
  case PIDtype::roll:
    return "roll";
  case PIDtype::yaw:
    return "yaw";
  case PIDtype::speed:
    return "speed";
  }
  return "?";
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
  Serial.setTimeout(2);
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
        String temp = command.substring(1);
        int ind = 0;
        while (ind < temp.length() && temp[ind] == ' ')
        {
          ++ind;
        }
        if (ind < temp.length() && isDigit(temp[ind]))
        {
          int ind2 = ind + 1;
          while (ind2 < temp.length() && temp[ind2] != ' ')
          {
            ++ind2;
          }
          if (ind2 < temp.length())
          {
            while (ind2 < temp.length() && temp[ind2] == ' ')
            {
              ++ind2;
            }
            if (ind2 < temp.length() && isDigit(temp[ind2]))
            {
              String arg1 = temp.substring(ind, ind2 - 1);
              String arg2 = temp.substring(ind2);
              stateChangeTimepoint = millis() + static_cast<unsigned long>(arg1.toFloat() * 1000);
              stateChangeTimepoint2 = stateChangeTimepoint + static_cast<unsigned long>(arg2.toFloat() * 1000);
            }
            else
            {
              stateChangeTimepoint = millis() + static_cast<unsigned long>(temp.toFloat() * 1000);
              stateChangeTimepoint2 = 0;
              pidPilotMessage.powerState = pidPilotMessage.powerState == PowerState::off ? PowerState::on : PowerState::off;
            }
          }
          else
          {
            stateChangeTimepoint = millis() + static_cast<unsigned long>(temp.toFloat() * 1000);
            stateChangeTimepoint2 = 0;
            pidPilotMessage.powerState = pidPilotMessage.powerState == PowerState::off ? PowerState::on : PowerState::off;
          }
        }
        else
        {
          stateChangeTimepoint = stateChangeTimepoint2 = 0;
          pidPilotMessage.powerState = pidPilotMessage.powerState == PowerState::off ? PowerState::on : PowerState::off;
        }
        break;
      }
      case 'h':
      {
        pidPilotMessage.pitchAngle = command.substring(1).toFloat();
        break;
      }
      case 'r':
      {
        pidPilotMessage.rollAngle = command.substring(1).toFloat();
        break;
      }
      case 'y':
      {
        pidPilotMessage.yawRate = command.substring(1).toFloat();
        break;
      }
      case 'v':
      {
        pidPilotMessage.verticalSpeed = command.substring(1).toFloat();
        break;
      }
      case 'c':
      {
        pidPilotMessage.compensation = command.substring(1).toFloat();
        if (pidPilotMessage.pid == PIDtype::roll)
        {
          rollCompensation = pidPilotMessage.compensation;
        }
        else if (pidPilotMessage.pid == PIDtype::pitch)
        {
          pitchCompensation = pidPilotMessage.compensation;
        }
        break;
      }
      case 't':
      {
        if (command.substring(2) == "pitch\n")
        {
          pidPilotMessage.pid = PIDtype::pitch;
          pidPilotMessage.P = pitchP;
          pidPilotMessage.I = pitchI;
          pidPilotMessage.D = pitchD;
          pidPilotMessage.compensation = pitchCompensation;
        }
        else if (command.substring(2) == "roll\n")
        {
          pidPilotMessage.pid = PIDtype::roll;
          pidPilotMessage.P = rollP;
          pidPilotMessage.I = rollI;
          pidPilotMessage.D = rollD;
          pidPilotMessage.compensation = rollCompensation;
        }
        else if (command.substring(2) == "yaw\n")
        {
          pidPilotMessage.pid = PIDtype::yaw;
          pidPilotMessage.P = yawP;
          pidPilotMessage.I = yawI;
          pidPilotMessage.D = yawD;
          pidPilotMessage.compensation = 0;
        }
        else if (command.substring(2) == "speed\n")
        {
          pidPilotMessage.pid = PIDtype::speed;
          pidPilotMessage.P = speedP;
          pidPilotMessage.I = speedI;
          pidPilotMessage.D = speedD;
          pidPilotMessage.compensation = 0;
        }
        break;
      }
      case 'p':
      {
        float P = command.substring(1).toFloat();
        switch (pidPilotMessage.pid)
        {
          case PIDtype::pitch:
            pitchP = P;
            break;
          case PIDtype::roll:
            rollP = P;
            break;
          case PIDtype::yaw:
            yawP = P;
            break;
          case PIDtype::speed:
            speedP = P;
            break;
        }
        pidPilotMessage.P = P;
        break;
      }
      case 'i':
      {
        float I = command.substring(1).toFloat();
        switch (pidPilotMessage.pid)
        {
          case PIDtype::pitch:
            pitchI = I;
            break;
          case PIDtype::roll:
            rollI = I;
            break;
          case PIDtype::yaw:
            yawI = I;
            break;
          case PIDtype::speed:
            speedI = I;
            break;
        }
        pidPilotMessage.I = I;
        break;
      }
      case 'd':
      {
        float D = command.substring(1).toFloat();
        switch (pidPilotMessage.pid)
        {
          case PIDtype::pitch:
            pitchD = D;
            break;
          case PIDtype::roll:
            rollD = D;
            break;
          case PIDtype::yaw:
            yawD = D;
            break;
          case PIDtype::speed:
            speedD = D;
            break;
        }
        pidPilotMessage.D = D;
        break;
      }
    }
  }

  if (stateChangeTimepoint != 0 && millis() > stateChangeTimepoint)
  {
    stateChangeTimepoint = 0;
    pidPilotMessage.powerState = pidPilotMessage.powerState == PowerState::off ? PowerState::on : PowerState::off;
  }

  if (stateChangeTimepoint2 != 0 && millis() > stateChangeTimepoint2)
  {
    stateChangeTimepoint2 = 0;
    pidPilotMessage.powerState = pidPilotMessage.powerState == PowerState::off ? PowerState::on : PowerState::off;
  }
  
  // send message to the drone
  if (nLoop % sendPeriod == 0)
  {
    Serial.print("connected: "); Serial.println(isConnected);
    Serial.print("power (S): "); Serial.print(powerStateToString(pidPilotMessage.powerState)); Serial.print("/"); Serial.println(powerStateToString(droneMessage.powerState));
    Serial.print("pitchAngle (H value): "); Serial.println(pidPilotMessage.pitchAngle, 10);
    Serial.print("rollAngle (R value): "); Serial.println(pidPilotMessage.rollAngle, 10);
    Serial.print("yawRate (Y value): "); Serial.println(pidPilotMessage.yawRate, 10);
    Serial.print("verticalSpeed (V value): "); Serial.println(pidPilotMessage.verticalSpeed, 10);
    Serial.print("PID type (T value): "); Serial.println(pidStateToString(pidPilotMessage.pid));
    Serial.print("P (P value): "); Serial.println(pidPilotMessage.P, 10);
    Serial.print("I (I value): "); Serial.println(pidPilotMessage.I, 10);
    Serial.print("D (D value): "); Serial.println(pidPilotMessage.D, 10);
    Serial.print("compensation (C value): "); Serial.println(pidPilotMessage.compensation, 10);
    ebyte32.send<PidPilotMessage, 1>(droneAddress, channel, pidPilotMessage);
  }

  ++nLoop;
  // wait for end of the cycle
  while(micros() - loopTime < loopPeriod);
  
}
