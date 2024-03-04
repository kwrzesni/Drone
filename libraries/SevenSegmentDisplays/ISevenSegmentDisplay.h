#ifndef I_SEVEN_SEGMENT_WITH_REGISTER
#define I_SEVEN_SEGMENT_WITH_REGISTER
#include "DiodePin.h"
#include "Arduino.h"

class ISevenSegmentDisplay
{
public:
  ISevenSegmentDisplay(DiodePin commonDiodePin);
  virtual void begin() = 0;
  virtual void setDigit(uint8 digit) const = 0;
  virtual ~ISevenSegmentDisplay() = default;
protected:
  const DiodePin commonDiodePin;
};

#endif