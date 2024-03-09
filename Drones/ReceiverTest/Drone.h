#ifndef DRONE
#define DRONE

//#define PID_MESSAGE

#include <Ebyte32.h>
#include <CommonDataTypes.h>

class Drone
{
public:
  Drone() = default;
  void begin();
  void step();
private:
  void handleRemoteControl();
  void waitTillEndOfStep();
private:
  // Peripherals
  Ebyte32 ebyte32 = Ebyte32{&Serial3, PB12};

  // Remote control
  const unsigned short PILOT_ADDRESS = 0x2137;
  const unsigned char CHANNEL = 5;
  #ifndef PID_MESSAGE
  PilotMessage pilotMessage = {};
  #else
  PidPilotMessage pidPilotMessage = {};
  #endif
  DroneMessage droneMessage = {PowerState::off};
  
  // Control loop
  const long STEP_TIME = 1000; // [μs]
  unsigned long stepStartTime = 0; // [μs]
};

#endif
