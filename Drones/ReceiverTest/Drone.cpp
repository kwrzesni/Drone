#include  "Drone.h"

String powerStateToString(PowerState powerState)
{
  switch (powerState)
  {
    case PowerState::off:
      return "off";
    case PowerState::on:
      return "on";
    case PowerState::none:
      return "--"; 
  }
  return "?";
}

String pidTypeToString(PIDtype pidType)
{
  switch (pidType)
  {
    case PIDtype::pitch:
      return "pitch";
    case PIDtype::roll:
      return "roll";
    case PIDtype::yaw:
      return "yaw";
    case PIDtype::speed:
      return "speed" ;
  }
  return "?";
}

void Drone::begin()
{
  // init remote communication
  ebyte32.begin();
  delay(2750);

  Serial.begin(9600);
}

void Drone::step()
{
  stepStartTime = micros();
  handleRemoteControl();
  waitTillEndOfStep();
}

void Drone::handleRemoteControl()
{
  #ifndef PID_MESSAGE
  if (ebyte32.avalibleToReceive<PilotMessage, 1>())
  {
    if (ebyte32.receive<PilotMessage, 1>(pilotMessage))
    {
      if (pilotMessage.powerState != PowerState::none)
      {
        droneMessage.powerState = pilotMessage.powerState;
      }
      ebyte32.send(PILOT_ADDRESS, CHANNEL, droneMessage);
      Serial.print("power state: "); Serial.println(powerStateToString(pilotMessage.powerState));
      Serial.print("roll angle: "); Serial.println(pilotMessage.rollAngle);
      Serial.print("pitch angle: "); Serial.println(pilotMessage.pitchAngle);
      Serial.print("yaw rate: "); Serial.println(pilotMessage.yawRate);
      Serial.print("vertical speed: "); Serial.println(pilotMessage.verticalSpeed);
    }
  }
  #else
  if (ebyte32.avalibleToReceive<PidPilotMessage, 1>())
  {
    if (ebyte32.receive<PidPilotMessage, 1>(pidPilotMessage))
    {
      if (pidPilotMessage.powerState != PowerState::none)
      {
        droneMessage.powerState = pidPilotMessage.powerState;
      }
      ebyte32.send(PILOT_ADDRESS, CHANNEL, droneMessage);
      Serial.print("power state: "); Serial.println(powerStateToString(pidPilotMessage.powerState));
      Serial.print("roll angle: "); Serial.println(pidPilotMessage.rollAngle);
      Serial.print("pitch angle: "); Serial.println(pidPilotMessage.pitchAngle);
      Serial.print("yaw rate: "); Serial.println(pidPilotMessage.yawRate);
      Serial.print("vertical speed: "); Serial.println(pidPilotMessage.verticalSpeed);
      Serial.print("pid type speed: "); Serial.println(pidTypeToString(pidPilotMessage.pid));
      Serial.print("P: "); Serial.println(pidPilotMessage.P);
      Serial.print("I: "); Serial.println(pidPilotMessage.I);
      Serial.print("D: "); Serial.println(pidPilotMessage.D);
      Serial.print("compensation: "); Serial.println(pidPilotMessage.compensation);
    }
  }
  #endif
}

void Drone::waitTillEndOfStep()
{
  while (micros() - stepStartTime < STEP_TIME);
}