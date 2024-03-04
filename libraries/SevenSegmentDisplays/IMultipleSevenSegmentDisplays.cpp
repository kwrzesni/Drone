#include "IMultipleSevenSegmentDisplays.h"
#include <HardwareTimer.h>

void IMultipleSevenSegmentDisplays::begin()
{
  if (!timerInitialized)
  {
    timer.setPeriod(UPDATE_PERIOD_MICROSECONDS);
    timer.attachInterrupt(TIMER_CHANNEL, updateDisplays);
    timer.resume();
    timerInitialized = true;
  }
  existingMultipleSevenSegmentDisplays.push_back(this);
}

void IMultipleSevenSegmentDisplays::updateDisplays()
{
  for (auto multipleSevenSegmentDisplays : existingMultipleSevenSegmentDisplays)
  {
    multipleSevenSegmentDisplays->update();
  }
}

IMultipleSevenSegmentDisplays::~IMultipleSevenSegmentDisplays()
{
  existingMultipleSevenSegmentDisplays.remove(this);
}


HardwareTimer IMultipleSevenSegmentDisplays::timer{TIMER_ID};
bool IMultipleSevenSegmentDisplays::timerInitialized = false;
std::list<IMultipleSevenSegmentDisplays*> IMultipleSevenSegmentDisplays::existingMultipleSevenSegmentDisplays{};