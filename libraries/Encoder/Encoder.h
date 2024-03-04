#ifndef ENCODER
#define ENCODER
#include "Arduino.h"

class Encoder
{
public:
  Encoder(uint8 pinA, uint8 pinB, int nPositions);
  void begin();
  void setPosition(long newPosition);
  long getPosition();
  long getModuloPosition();
  long getAngleInDegree();
  long getModuloAngleInDegree();
  float getAngle();
  float getModuloAngle();
  void tick();
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
};

#endif
