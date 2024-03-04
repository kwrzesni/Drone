#include "Joystick.h"

Joystick::Joystick(uint8 rxPin, uint8 ryPin, int xCenter, int yCenter)
  : rxPin{rxPin}, ryPin{ryPin}, xCenter{xCenter}, yCenter{yCenter}
{}

void Joystick::begin()
{
  pinMode(rxPin, INPUT);
  pinMode(ryPin, INPUT);
}

Joystick::Vector Joystick::getVector() const
{
  int x = readX();
  int y = readY();
  if ((-EPSILON < x && x < EPSILON) && (-EPSILON < y && y < EPSILON))
  {
    return {0, 0};
  }
  double norm = sqrt(static_cast<double>(x * x + y * y));
  return {x / norm, y / norm};
}

int Joystick::readX() const
{
  return analogRead(rxPin) - xCenter;
}

int Joystick::readY() const
{
  return analogRead(ryPin) - yCenter;
}