#include "Gimbal1Axis.h"

Gimbal1Axis::Gimbal1Axis(const int pin, int minValue, int maxValue)
  : pin{pin}, center{(maxValue + minValue) / 2.0f}, length{(maxValue - minValue) / 2.0f}
{}

void Gimbal1Axis::begin() const
{
  pinMode(pin, INPUT);
}

float Gimbal1Axis::read() const
{
  return max(-1.0f, min((analogRead(pin) - center) / length, 1.0f));
}