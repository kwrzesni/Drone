#ifndef ANALOG_DIODE
#include "Arduino.h"

class AnalogDiode
{
public:
  AnalogDiode(uint8 pin, float value = 0.0f, int maxValue = 255);
  void begin();
  void turnOn();
  void turnOff();
  void setValue(float value);
  float getValue();
private:
  const uint8 pin;
  float value;
  int maxValue;
  int integerValue;
};

#endif
