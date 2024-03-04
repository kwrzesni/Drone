#include "Motor.h"

Motor::Motor(const int pin, float speed)
  : pin{pin}, speed{speed}
{
}

void Motor::begin()
{
  ESC.attach(pin, 1000, 2000);
  ESC.write(int(180 * speed));
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