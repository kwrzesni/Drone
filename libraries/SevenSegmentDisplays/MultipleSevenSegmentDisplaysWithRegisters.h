#ifndef MULTIPLE_SEVEN_SEGMENT_DISPLAYS_WITH_REGISTERS
#define MULTIPLE_SEVEN_SEGMENT_DISPLAYS_WITH_REGISTERS
#include "BaseMultipleSevenSegmentDisplays.h"
#include "SevenSegmentDisplayConstants.h"
#include "DiodePin.h"
#include "Arduino.h"

template<uint8 N_DISPLAYS>
class MultipleSevenSegmentDisplaysWithRegisters : public BaseMultipleSevenSegmentDisplays<N_DISPLAYS>
{
public:
  MultipleSevenSegmentDisplaysWithRegisters(uint8 pinSHCP, uint8 pinSTCP, uint8 pinDS, DiodePin commonDiodePin = DiodePin::anode);
  void begin() override;
  void update() override;
  ~MultipleSevenSegmentDisplaysWithRegisters() override = default;
private:
  const uint8 pinSHCP;
  const uint8 pinSTCP;
  const uint8 pinDS;
  using BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::currentDisplay;
  using BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::symbols;
  using BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::commonDiodePin;
};

template<uint8 N_DISPLAYS>
MultipleSevenSegmentDisplaysWithRegisters<N_DISPLAYS>::MultipleSevenSegmentDisplaysWithRegisters(uint8 pinSHCP, uint8 pinSTCP, uint8 pinDS, DiodePin commonDiodePin)
  : BaseMultipleSevenSegmentDisplays<N_DISPLAYS>{commonDiodePin}, pinSHCP{pinSHCP}, pinSTCP{pinSTCP}, pinDS{pinDS}
{
}

template<uint8 N_DISPLAYS>
void MultipleSevenSegmentDisplaysWithRegisters<N_DISPLAYS>::begin()
{
  IMultipleSevenSegmentDisplays::begin();
  pinMode(pinSHCP, OUTPUT);
  pinMode(pinSTCP, OUTPUT);
  pinMode(pinDS, OUTPUT);
  digitalWrite(pinSTCP, LOW);
  shiftOut(pinDS, pinSHCP, MSBFIRST, 0);
  shiftOut(pinDS, pinSHCP, MSBFIRST, 0);
  digitalWrite(pinSTCP, HIGH);

  BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::setIntegerValue(0);
}

template<uint8 N_DISPLAYS>
void MultipleSevenSegmentDisplaysWithRegisters<N_DISPLAYS>::update()
{
  currentDisplay = (currentDisplay + 1) % N_DISPLAYS;
  if (commonDiodePin == DiodePin::anode)
  {
    digitalWrite(pinSTCP, LOW);
    shiftOut(pinDS, pinSHCP, MSBFIRST, 1u << (N_DISPLAYS - 1 - currentDisplay));
    shiftOut(pinDS, pinSHCP, LSBFIRST, ~SYMBOL_TO_SEGMENTS_REGISTER[symbols[currentDisplay]]);
    digitalWrite(pinSTCP, HIGH);
  }
  else
  {
    digitalWrite(pinSTCP, LOW);
    shiftOut(pinDS, pinSHCP, MSBFIRST, ~(1u << (N_DISPLAYS - 1 - currentDisplay)));
    shiftOut(pinDS, pinSHCP, LSBFIRST, SYMBOL_TO_SEGMENTS_REGISTER[symbols[currentDisplay]]);
    digitalWrite(pinSTCP, HIGH);
  }
}

#endif
