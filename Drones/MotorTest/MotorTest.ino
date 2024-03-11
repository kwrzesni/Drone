#include "Drone.h"

Drone drone;

void setup() 
{
  drone.begin();
}

void loop() 
{
  drone.step();
}
