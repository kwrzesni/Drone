#ifndef JOYSTICK_WITH_BUTTON_CALIBRATION
#define JOYSTICK_WITH_BUTTON_CALIBRATION
#include "Arduino.h"

class JoystickWithButtonCalibration
{
public:
  JoystickWithButtonCalibration(uint8 rxPin, uint8 ryPin, uint8 pinButton);
  void begin();
  void test();
  uint8 readButtonState();
  int readX() const;
  int readY() const;
private:
  const uint8 rxPin;
  const uint8 ryPin;
  static constexpr int EPSILON = 100;
  int sumX = 0;
  int sumY = 0;
  int nMeasurments = 0;

  const uint8 pinButton;
  uint8 buttonState = 0;
  uint8 lastButtonState = 0;
  unsigned long lastDebouncingTime = 0;
  static constexpr unsigned long DEBOUNCING_DELAY = 30;
};

#endif
