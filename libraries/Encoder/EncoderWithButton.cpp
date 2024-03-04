#include "EncoderWithButton.h"

const int8_t KNOBDIR[] = {
                          0 , -1,  1,  0,
                          1 ,  0,  0, -1,
                          -1,  0,  0,  1,
                          0 ,  1, -1,  0
                         };

void tickEncoderWithButton(void* encoderWithButton)
{
  reinterpret_cast<EncoderWithButton*>(encoderWithButton)->tick();
}

EncoderWithButton::EncoderWithButton(uint8 pinA, uint8 pinB, int nPositions, uint8 pinButton)
  : pinA{pinA}, pinB{pinB}, nPositions{nPositions}, pinButton{pinButton}
{}

void EncoderWithButton::begin()
{
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(pinA), tickEncoderWithButton, reinterpret_cast<void*>(this), CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), tickEncoderWithButton, reinterpret_cast<void*>(this), CHANGE);
  pinMode(pinButton, INPUT_PULLDOWN);
}

long EncoderWithButton::getPosition()
{
  tick();
  return positionExt;
}

void EncoderWithButton::setPosition(long newPosition)
{
    position = ((newPosition << 1) | (position & 0x01L));
    positionExt = newPosition;
    positionExtPrev = newPosition;
}

long EncoderWithButton::getModuloPosition()
{
  long out = getPosition() % nPositions;
  return out >= 0 ? out : out + nPositions;
}

long EncoderWithButton::getAngleInDegree()
{
  return 360 / nPositions * getPosition();
}

long EncoderWithButton::getModuloAngleInDegree()
{
  return 360 / nPositions * getModuloPosition();
}

float EncoderWithButton::getAngle()
{
  return 6.28318530718 / nPositions * getPosition();
}

float EncoderWithButton::getModuloAngle()
{
  return 6.28318530718 / nPositions * getModuloPosition();
}

void EncoderWithButton::tick()
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

uint8 EncoderWithButton::readButtonState()
{
  uint8 readingPushbutton = digitalRead(pinButton);

  if (readingPushbutton != lastButtonState) 
  {
    lastDebouncingTime = millis();
  }

  if ((millis() - lastDebouncingTime) > DEBOUNCING_DELAY) 
  {
    if (buttonState != readingPushbutton)
    {
      buttonState = readingPushbutton;
    }
  }
  lastButtonState = readingPushbutton;
  return !buttonState;
}