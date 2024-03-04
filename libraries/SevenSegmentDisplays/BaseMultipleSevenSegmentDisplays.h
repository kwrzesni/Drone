#ifndef BASE_MULTIPLE_SEVEN_SEGMENT_DISPLAYS
#define BASE_MULTIPLE_SEVEN_SEGMENT_DISPLAYS
#include "IMultipleSevenSegmentDisplays.h"
#include "DiodePin.h"
#include "Arduino.h"

template<uint8 N_DISPLAYS>
class BaseMultipleSevenSegmentDisplays : public IMultipleSevenSegmentDisplays
{
public:
  BaseMultipleSevenSegmentDisplays(DiodePin commonDiodePin);
  void setIntegerValue(int value) override;
  void setValue(double value) override;
  virtual ~BaseMultipleSevenSegmentDisplays() = default;
protected:
  const DiodePin commonDiodePin;
  char symbols[N_DISPLAYS] = {};
  uint8 currentDisplay = 0;
private:
  static constexpr int UPPER_LIMIT = pow(10, N_DISPLAYS);
  static constexpr int LOWER_LIMIT = -pow(10, N_DISPLAYS - 1);
};

template<uint8 N_DISPLAYS>
BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::BaseMultipleSevenSegmentDisplays(DiodePin commonDiodePin)
  : commonDiodePin{commonDiodePin}
{}

template<uint8 N_DISPLAYS>
void BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::setIntegerValue(int value)
{
  if (value >= UPPER_LIMIT)
  {
    value = UPPER_LIMIT - 1;
  }
  if (value <= LOWER_LIMIT)
  {
    value = LOWER_LIMIT + 1;
  }
  bool isNegative = value < 0;
  if (isNegative)
  {
    symbols[N_DISPLAYS - 1] = '-';
    value = -value;
  }
  for (uint8 i = 0; i < N_DISPLAYS - static_cast<uint8>(isNegative); ++i)
  {
    symbols[i] = value % 10 + '0';
    value /= 10;
  }
}
  
template<uint8 N_DISPLAYS>
void BaseMultipleSevenSegmentDisplays<N_DISPLAYS>::setValue(double value)
{
  if (value >= 0)
  {
    if (value >= UPPER_LIMIT)
    {
      setIntegerValue(UPPER_LIMIT - 1);
      return;
    }
    int limit = UPPER_LIMIT / 10;
    if (value >= limit)
    {
      setIntegerValue(static_cast<int>(value));
      return;
    }
    limit /= 10;
    for (uint8 dp = 1; dp < N_DISPLAYS; ++dp, limit /= 10)
    {
      if (value >= limit)
      {
        setIntegerValue(static_cast<int>(value * pow(10, dp)));
        symbols[dp] += '.';
        return;
      }
    }
    setIntegerValue(static_cast<int>(value * pow(10, N_DISPLAYS - 1)));
    symbols[N_DISPLAYS - 1] += '.';
    return;
  }
  else
  {
    if (value <= LOWER_LIMIT)
    {
      setIntegerValue(LOWER_LIMIT + 1);
      return;
    }
    int limit = LOWER_LIMIT / 10;
    if (value <= limit)
    {
      setIntegerValue(static_cast<int>(value));
      return;
    }
    limit /= 10;
    for (uint8 dp = 1; dp < N_DISPLAYS - 1; ++dp, limit /= 10)
    {
      if (value <= limit)
      {
        setIntegerValue(static_cast<int>(value * pow(10, dp)));
        symbols[dp] += '.';
        return;
      }
    }
    setIntegerValue(static_cast<int>(value * pow(10, N_DISPLAYS - 2)));
    symbols[N_DISPLAYS - 2] += '.';
    return;
  }
}

#endif
