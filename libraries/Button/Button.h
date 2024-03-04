#ifndef BUTTON
#define BUTTON
#include "Arduino.h"

class Button
{
public:
  Button(uint8 pin);
  void begin();
  uint8 read();
private:
  uint8 pin;
  uint8 buttonState = 0;
  uint8 lastButtonState = 0;
  unsigned long lastDebouncingTime = 0;
  static constexpr unsigned long DEBOUNCING_DELAY = 30;
};

#endif
