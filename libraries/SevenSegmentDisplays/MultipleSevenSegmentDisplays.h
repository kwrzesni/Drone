#ifndef MULTIPLE_SEVEN_SEGMENT_DISPLAYS
#define MULTIPLE_SEVEN_SEGMENT_DISPLAYS
#include "BaseMultipleSevenSegmentDisplays.h"
#include "SevenSegmentDisplayConstants.h"
#include "Arduino.h"
#include <array>

template<uint8 N_DISPLAYS>
class MultipleSevenSegmentDisplays : public BaseMultipleSevenSegmentDisplays<N_DISPLAYS>
{
public:
  MultipleSevenSegmentDisplays(uint8 pinA, uint8 pinB, uint8 pinC, uint8 pinD, uint8 pinE, uint8 pinF, uint8 pinG, uint8 pinDP, const std::array<uint8, N_DISPLAYS>& displayPins, DiodePin commonDiodePin = DiodePin::anode);
  void begin() override;
  void update() override;
  ~MultipleSevenSegmentDisplays() override = default;
private:
  const std::array<uint8, 8> segmentsPins;
  const std::array<uint8, N_DISPLAYS> displayPins;
  using BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::currentDisplay;
  using BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::symbols;
  using BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::commonDiodePin;
};

template<uint8 N_DISPLAYS>
MultipleSevenSegmentDisplays<N_DISPLAYS>::MultipleSevenSegmentDisplays(uint8 pinA, uint8 pinB, uint8 pinC, uint8 pinD, uint8 pinE, uint8 pinF, uint8 pinG, uint8 pinDP, const std::array<uint8, N_DISPLAYS>& displayPins, DiodePin commonDiodePin)
  : BaseMultipleSevenSegmentDisplays<N_DISPLAYS>{commonDiodePin}, segmentsPins{pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP}, displayPins(displayPins)
{
}

template<uint8 N_DISPLAYS>
void MultipleSevenSegmentDisplays<N_DISPLAYS>::begin()
{
  IMultipleSevenSegmentDisplays::begin();
  for (uint8 pin : segmentsPins)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  for (uint8 pin : displayPins)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::setIntegerValue(0);
}

template<uint8 N_DISPLAYS>
void MultipleSevenSegmentDisplays<N_DISPLAYS>::update()
{
  if (commonDiodePin == DiodePin::anode)
  {
    digitalWrite(displayPins[N_DISPLAYS - 1 - currentDisplay], LOW);
    currentDisplay = (currentDisplay + 1) % N_DISPLAYS;
    digitalWrite(displayPins[N_DISPLAYS - 1 - currentDisplay], HIGH);
    for (uint8 i = 0; i < 8; ++i)
    {
      digitalWrite(segmentsPins[i], !SYMBOL_TO_SEGMENTS[symbols[currentDisplay]][i]);
    }
  }
  else
  {
    digitalWrite(displayPins[N_DISPLAYS - 1 - currentDisplay], HIGH);
    currentDisplay = (currentDisplay + 1) % N_DISPLAYS;
    digitalWrite(displayPins[N_DISPLAYS - 1 - currentDisplay], LOW);
    for (uint8 i = 0; i < 8; ++i)
    {
      digitalWrite(segmentsPins[i], SYMBOL_TO_SEGMENTS[symbols[currentDisplay]][i]);
    }
  }
}

#endif
