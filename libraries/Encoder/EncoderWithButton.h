#ifndef ENCODER_WITH_BUTTON
#define ENCODER_WITH_BUTTON
#include "Arduino.h"

class EncoderWithButton
{
public:
  EncoderWithButton(uint8 pinA, uint8 pinB, int nPositions, uint8 pinButton);
  void begin();
  void setPosition(long newPosition);
  long getPosition();
  long getModuloPosition();
  long getAngleInDegree();
  long getModuloAngleInDegree();
  float getAngle();
  float getModuloAngle();
  void tick();
  uint8 readButtonState();
private:
  const uint8 pinA;
  const uint8 pinB;
  const int nPositions;

  volatile int8_t oldState;
  volatile long position;
  volatile long positionExt;
  volatile long positionExtPrev;
  unsigned long positionExtTime;
  unsigned long positionExtTimePrev;

  const uint8 pinButton;
  uint8 buttonState = 0;
  uint8 lastButtonState = 0;
  unsigned long lastDebouncingTime = 0;
  static constexpr unsigned long DEBOUNCING_DELAY = 30;
};

#endif
