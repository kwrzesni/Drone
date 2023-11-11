#include "Motor.h"

Motor::Motor(const int pin)
  : pin{pin}
{
  setSpeed(0);
}

void Motor::begin()
{
  ESC.attach(pin, 1000, 2000);
}

void Motor::setSpeed(float value)
{
  speed = value;
  ESC.write(int(180 * speed));
}

float Motor::getSpeed() const
{
  return speed;
}