#ifndef JOYSTICK_WITH_BUTTON
#define JOYSTICK_WITH_BUTTON
#include "Arduino.h"

class JoystickWithButton
{
public:
  struct Vector
  {
    float x;
    float y;
  };
public:
  JoystickWithButton(uint8 rxPin, uint8 ryPin, int xCenter, int yCenter, uint8 pinButton);
  void begin();
  Vector getVector() const;
  uint8 readButtonState();
private:
  int readX() const;
  int readY() const;
private:
  const uint8 rxPin;
  const uint8 ryPin;
  const int xCenter;
  const int yCenter;
  static constexpr int EPSILON = 100;

  const uint8 pinButton;
  uint8 buttonState = 0;
  uint8 lastButtonState = 0;
  unsigned long lastDebouncingTime = 0;
  static constexpr unsigned long DEBOUNCING_DELAY = 30;
};

#endif
