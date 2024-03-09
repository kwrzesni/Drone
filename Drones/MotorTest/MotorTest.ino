#include <Motor.h>

Motor motor0 = Motor{PA3};
Motor motor1 = Motor{PA0};
Motor motor2 = Motor{PA1};
Motor motor3 = Motor{PA2};
float minimumSpeed = 0.04;

void setup() 
{
  motor0.begin();
  motor1.begin();
  motor2.begin();
  motor3.begin();
  delay(5000);
  motor0.setSpeed(minimumSpeed);
  motor1.setSpeed(minimumSpeed);
  motor2.setSpeed(minimumSpeed);
  motor3.setSpeed(minimumSpeed);
  delay(5000);
  motor0.setSpeed(0.0);
  motor1.setSpeed(0.0);
  motor2.setSpeed(0.0);
  motor3.setSpeed(0.0);
}

void loop() 
{
}
