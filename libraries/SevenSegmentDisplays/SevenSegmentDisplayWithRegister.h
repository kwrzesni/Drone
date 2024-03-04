#ifndef SEVEN_SEGMENT_WITH_REGISTER
#define SEVEN_SEGMENT_WITH_REGISTER
#include "ISevenSegmentDisplay.h"
#include "Arduino.h"

class SevenSegmentDisplayWithRegister : public ISevenSegmentDisplay
{
public:
  SevenSegmentDisplayWithRegister(uint8 pinSHCP, uint8 pinSTCP, uint8 pinDS, DiodePin commonDiodePin = DiodePin::anode);
  void begin() override;
  void setDigit(uint8 digit) const override;
private:
  const uint8 pinSHCP;
  const uint8 pinSTCP;
  const uint8 pinDS;
};

#endif
