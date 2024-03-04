#include "Gimbal1AxisCalibration.h"

Gimbal1AxisCalibration::Gimbal1AxisCalibration(const int pin)
  : pin{pin}
{}

void Gimbal1AxisCalibration::begin() const
{
  Serial.begin(9600);
  pinMode(pin, INPUT);
}

int Gimbal1AxisCalibration::read()
{
  return analogRead(pin);
}