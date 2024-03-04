#include "JoystickWithButton.h"

JoystickWithButton::JoystickWithButton(uint8 rxPin, uint8 ryPin, int xCenter, int yCenter, uint8 pinButton)
  : rxPin{rxPin}, ryPin{ryPin}, xCenter{xCenter}, yCenter{yCenter}, pinButton{pinButton}
{}

void JoystickWithButton::begin()
{
  pinMode(rxPin, INPUT);
  pinMode(ryPin, INPUT);
  pinMode(pinButton, INPUT_PULLUP);
}

JoystickWithButton::Vector JoystickWithButton::getVector() const
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

int JoystickWithButton::readX() const
{
  return analogRead(rxPin) - xCenter;
}

int JoystickWithButton::readY() const
{
  return analogRead(ryPin) - yCenter;
}

uint8 JoystickWithButton::readButtonState()
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