#ifndef GIMBAL_CALIBRATION
#define GIMBAL_CALIBRATION
#include <Arduino.h>
#include <limits>

class Gimbal1AxisCalibration
{
public:
  Gimbal1AxisCalibration(const int pin);
  void begin() const;
  int read();
private:
  const uint8 pin;
};

#endif
