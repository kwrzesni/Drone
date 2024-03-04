#include "JoystickCalibration.h"

JoystickCalibration::JoystickCalibration(uint8 rxPin, uint8 ryPin)
  : rxPin{rxPin}, ryPin{ryPin}
{}

void JoystickCalibration::begin()
{
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(ryPin, INPUT);
}

void JoystickCalibration::test()
{
  sumX += readX();
  sumY += readY();
  ++nMeasurments;
  Serial.print("Joystick - x: "); Serial.print(sumX / nMeasurments); Serial.print(", y: "); Serial.println(sumY / nMeasurments);
}

int JoystickCalibration::readX() const
{
  return analogRead(rxPin);
}

int JoystickCalibration::readY() const
{
  return analogRead(ryPin);
}