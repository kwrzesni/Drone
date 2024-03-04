#ifndef GIMBAL
#define GIMBAL
#include <Arduino.h>

class Gimbal1Axis
{
public:
  Gimbal1Axis(const int pin, int minValue, int maxValue);
  void begin() const;
  float read() const;
private:
  const uint8 pin;
  const float center;
  const float length;
};

#endif
