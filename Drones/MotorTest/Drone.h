#ifndef DRONE
#define DRONE

class Drone
{
private:
  enum class State
  {
    off = 0,
    userControlled = 1,
    disconnected = 2,
    longTimeDisconnected = 3
  };
public:
  Drone() = default;
  void begin();
  void step();
private:
  void setMotorTimers();
  void setMotorsSpeed();
  void setMotorsPWM();
  void waitTillEndOfStep();
private:
  // Control loop
  const long STEP_TIME = 4000; // [μs]
  long stepStartTime = 0;  // [μs]
  long testStartTime = 0;  // [μs]
  float motor0Speed = 0.0f;
  float motor1Speed = 0.0f;
  float motor2Speed = 0.0f;
  float motor3Speed = 0.0f;
};

#endif
