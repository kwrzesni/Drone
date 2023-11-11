#ifndef MOTOR
#define MOTOR
#include "Arduino.h"
#include <Servo.h>

class Motor
{
public:
  Motor(const int pin);
  void begin();
  void setSpeed(float value);
  float getSpeed() const;
private:
  const int pin;
  float speed = 0.0f;
  Servo ESC;
};

#endif
