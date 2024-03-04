#include "Encoder.h"

const int8_t KNOBDIR[] = {
                          0 , -1,  1,  0,
                          1 ,  0,  0, -1,
                          -1,  0,  0,  1,
                          0 ,  1, -1,  0
                         };

void tickEncoder(void* encoder)
{
  reinterpret_cast<Encoder*>(encoder)->tick();
}

Encoder::Encoder(uint8 pinA, uint8 pinB, int nPositions)
  : pinA{pinA}, pinB{pinB}, nPositions{nPositions}
{}

void Encoder::begin()
{
  attachInterrupt(digitalPinToInterrupt(pinA), tickEncoder, reinterpret_cast<void*>(this), CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), tickEncoder, reinterpret_cast<void*>(this), CHANGE);
}

long Encoder::getPosition()
{
  tick();
  return positionExt;
}

void Encoder::setPosition(long newPosition)
{
    position = ((newPosition << 1) | (position & 0x01L));
    positionExt = newPosition;
    positionExtPrev = newPosition;
}

long Encoder::getModuloPosition()
{
  long out = getPosition() % nPositions;
  return out >= 0 ? out : out + nPositions;
}

long Encoder::getAngleInDegree()
{
  return 360 / nPositions * getPosition();
}

long Encoder::getModuloAngleInDegree()
{
  return 360 / nPositions * getModuloPosition();
}

float Encoder::getAngle()
{
  return 6.28318530718 / nPositions * getPosition();
}

float Encoder::getModuloAngle()
{
  return 6.28318530718 / nPositions * getModuloPosition();
}

void Encoder::tick()
{
  int sig1 = digitalRead(pinA);
  int sig2 = digitalRead(pinB);
  int8_t thisState = sig1 | (sig2 << 1);

  if (oldState != thisState) 
  {
    position += KNOBDIR[thisState | (oldState << 2)];
    oldState = thisState;

    if ((thisState == 0) || (thisState == 3)) 
    {

      positionExt = position >> 1;
      positionExtTimePrev = positionExtTime;
      positionExtTime = millis();
    }
  }
}