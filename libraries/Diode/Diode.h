#ifndef DIODE
#include "Arduino.h"

class Diode
{
public:
  Diode(uint8 pin, uint8 value = LOW);
  void begin();
  void turnOn();
  void turnOff();
  void toogle();
  void setValue(uint8 value);
  uint8 getValue();
private:
  const uint8 pin;
  uint8 value;
};

#endif
