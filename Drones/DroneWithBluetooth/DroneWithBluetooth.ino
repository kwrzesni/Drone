#include "Drone.h"

Drone drone;

void setup() 
{
  Serial.begin(9600);
  drone.begin();
}

void loop() 
{
  drone.step();
}
