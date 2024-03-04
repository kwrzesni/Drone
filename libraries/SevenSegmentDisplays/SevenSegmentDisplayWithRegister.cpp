#include "SevenSegmentDisplayWithRegister.h"
#include "SevenSegmentDisplayConstants.h"

SevenSegmentDisplayWithRegister::SevenSegmentDisplayWithRegister(uint8 pinSHCP, uint8 pinSTCP, uint8 pinDS, DiodePin commonDiodePin)
  : ISevenSegmentDisplay{commonDiodePin}, pinSHCP{pinSHCP}, pinSTCP{pinSTCP}, pinDS{pinDS}
{
}

void SevenSegmentDisplayWithRegister::begin()
{
  pinMode(pinSHCP, OUTPUT);
  pinMode(pinSTCP, OUTPUT);
  pinMode(pinDS, OUTPUT);
  digitalWrite(pinSTCP, LOW);
  shiftOut(pinDS, pinSHCP, MSBFIRST, 0);
  digitalWrite(pinSTCP, HIGH);

  setDigit(0);
}

void SevenSegmentDisplayWithRegister::setDigit(uint8 digit) const
{
  if (digit > 9)
  {
    digit = 9;
  }
  if (commonDiodePin == DiodePin::anode)
  {
    digitalWrite(pinSTCP, LOW);
    shiftOut(pinDS, pinSHCP, LSBFIRST, ~SYMBOL_TO_SEGMENTS_REGISTER[digit + '0']);
    digitalWrite(pinSTCP, HIGH);
  }
  else
  {
    digitalWrite(pinSTCP, LOW);
    shiftOut(pinDS, pinSHCP, LSBFIRST, SYMBOL_TO_SEGMENTS_REGISTER[digit + '0']);
    digitalWrite(pinSTCP, HIGH);
  }
}