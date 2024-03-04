#ifndef JOYSTICK_CALIBRATION
#define JOYSTICK_CALIBRATION
#include "Arduino.h"

class JoystickCalibration
{
public:
  JoystickCalibration(uint8 rxPin, uint8 ryPin);
  void begin();
  void test();
private:
  int readX() const;
  int readY() const;
private:
  const uint8 rxPin;
  const uint8 ryPin;
  static constexpr int EPSILON = 100;
  int sumX = 0;
  int sumY = 0;
  int nMeasurments = 0;
};

#endif
