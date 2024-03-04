#include "JoystickWithButtonCalibration.h"

JoystickWithButtonCalibration::JoystickWithButtonCalibration(uint8 rxPin, uint8 ryPin, uint8 pinButton)
  : rxPin{rxPin}, ryPin{ryPin}, pinButton{pinButton}
{}

void JoystickWithButtonCalibration::begin()
{
  Serial.begin(9600);
  pinMode(rxPin, INPUT);
  pinMode(ryPin, INPUT);
  pinMode(pinButton, INPUT_PULLUP);
}

void JoystickWithButtonCalibration::test()
{
  sumX += readX();
  sumY += readY();
  ++nMeasurments;
  Serial.print("Joystick - x: "); Serial.print(sumX / nMeasurments); Serial.print(", y: "); Serial.println(sumY / nMeasurments);
}

int JoystickWithButtonCalibration::readX() const
{
  return analogRead(rxPin);
}

int JoystickWithButtonCalibration::readY() const
{
  return analogRead(ryPin);
}

uint8 JoystickWithButtonCalibration::readButtonState()
{
  uint8 readingPushbutton = digitalRead(pinButton);

  if (readingPushbutton != lastButtonState) 
  {
    lastDebouncingTime = millis();
  }

  if ((millis() - lastDebouncingTime) > DEBOUNCING_DELAY) 
  {
    if (buttonState != readingPushbutton)
    {
      buttonState = readingPushbutton;
    }
  }
  lastButtonState = readingPushbutton;
  return !buttonState;
}