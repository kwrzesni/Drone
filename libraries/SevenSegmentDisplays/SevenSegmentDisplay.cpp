#include "SevenSegmentDisplay.h"

SevenSegmentDisplay::SevenSegmentDisplay(uint8 pinA, uint8 pinB, uint8 pinC, uint8 pinD, uint8 pinE, uint8 pinF, uint8 pinG, uint8 pinDP, DiodePin commonDiodePin)
  : ISevenSegmentDisplay{commonDiodePin}, segmentsPins{pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP}
{
}

void SevenSegmentDisplay::begin()
{
  for (uint8 pin : segmentsPins)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  setDigit(0);
}

void SevenSegmentDisplay::setDigit(uint8 digit) const
{
  if (digit > 9)
  {
    digit = 9;
  }
  if (commonDiodePin == DiodePin::anode)
  {
    for (uint8 i = 0; i < 8; ++i)
    {
      digitalWrite(segmentsPins[i], !SYMBOL_TO_SEGMENTS[digit + '0'][i]);
    }
  }
  else
  {
    for (uint8 i = 0; i < 8; ++i)
    {
      digitalWrite(segmentsPins[i], SYMBOL_TO_SEGMENTS[digit + '0'][i]);
    }
  }
}