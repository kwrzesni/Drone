#ifndef I_MULTIPLE_SEVEN_SEGMENT_DISPLAYS
#define I_MULTIPLE_SEVEN_SEGMENT_DISPLAYS
#include "Arduino.h"
#include <HardwareTimer.h>
#include <list>


class IMultipleSevenSegmentDisplays
{
public:
  IMultipleSevenSegmentDisplays() = default;
  virtual void begin();
  virtual void setIntegerValue(int value) = 0;
  virtual void setValue(double value) = 0;
  virtual ~IMultipleSevenSegmentDisplays();
protected:
  virtual void update() = 0;
private:
  static void updateDisplays();
private:
  static constexpr uint8 TIMER_ID = 1;
  static constexpr uint8 TIMER_CHANNEL = 1;
  static bool timerInitialized;
  static HardwareTimer timer;
  static constexpr uint32 UPDATE_PERIOD_MICROSECONDS = 6000;
  static std::list<IMultipleSevenSegmentDisplays*> existingMultipleSevenSegmentDisplays;
};

#endif
