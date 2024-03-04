#ifndef SEVEN_SEGMENT_DISPLAY
#define SEVEN_SEGMENT_DISPLAY
#include "ISevenSegmentDisplay.h"
#include "SevenSegmentDisplayConstants.h"
#include "Arduino.h"
#include <array>

class SevenSegmentDisplay : public ISevenSegmentDisplay
{
public:
  SevenSegmentDisplay(uint8 pinA, uint8 pinB, uint8 pinC, uint8 pinD, uint8 pinE, uint8 pinF, uint8 pinG, uint8 pinDP, DiodePin commonDiodePin = DiodePin::anode);
  void begin() override;
  void setDigit(uint8 digit) const;
private:
  const std::array<uint8, 8> segmentsPins;
};

#endif
