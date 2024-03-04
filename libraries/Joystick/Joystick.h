#ifndef JOYSTICK
#define JOYSTICK
#include "Arduino.h"

class Joystick
{
public:
  struct Vector
  {
    float x;
    float y;
  };
public:
  Joystick(uint8 rxPin, uint8 ryPin, int xCenter, int yCenter);
  void begin();
  Vector getVector() const;
private:
  int readX() const;
  int readY() const;
private:
  const uint8 rxPin;
  const uint8 ryPin;
  const int xCenter;
  const int yCenter;
  static constexpr int EPSILON = 100;
};

#endif
