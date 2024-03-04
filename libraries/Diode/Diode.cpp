#include "Diode.h"

Diode::Diode(uint8 pin, uint8 value)
  : pin{pin}, value{value}
{}

void Diode::begin()
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, value);
}

void Diode::turnOn()
{
  setValue(HIGH);
}

void Diode::turnOff()
{
  setValue(LOW);
}

void Diode::toogle()
{
  setValue(value == LOW ? HIGH : LOW);
}

void Diode::setValue(uint8 value)
{
  this->value = value;
  digitalWrite(pin, value);
}

uint8 Diode::getValue()
{
  return value;
}