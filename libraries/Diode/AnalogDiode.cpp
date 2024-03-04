#include "AnalogDiode.h"

AnalogDiode::AnalogDiode(uint8 pin, float value, int maxValue)
  : pin{pin}, value{value}, maxValue{maxValue}
{}
  
void AnalogDiode::begin()
{
  pinMode(pin, OUTPUT);
  setValue(value);
}

void AnalogDiode::turnOn()
{
  setValue(1.0f);
}

void AnalogDiode::turnOff()
{
  setValue(0.0f);
}

void AnalogDiode::setValue(float value)
{
  this->value = max(0.0, min(value, 1.0f));
  integerValue = round(maxValue * this->value);
  analogWrite(pin, integerValue);
}

float AnalogDiode::getValue()
{
  return value;
}