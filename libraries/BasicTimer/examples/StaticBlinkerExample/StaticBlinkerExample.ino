#include <BasicTimer.h>

//The Blinkers are useful for blinking LEDs or anything else that you want
//turned on and off in rhythm using true/false values 

//Create a static blinker.  The blink time is supplied via template 
//argument so it takes up no space in memory.  This reduces the 
//memory footprint of the blinker to only 5 bytes from 9 bytes.
//This is useful if you never want the blink time to change.
StaticBlinker<250> staticBlinkerExample;

void setup() {
  Serial.begin(9600);
  //Resets the blinker (resets its timer and starts it at false)
  //This isn't necessary, but could be useful to roughly sync the timing
  //of multiple blinkers
  staticBlinkerExample.reset();

  //If we want the blinker to start on/true instead of off/false we 
  //can assign it's value directly.  It will toggle every time
  //it's internal timer expires
  staticBlinkerExample = true;

  Serial.print("We have create a blinker with blink time: ");
  Serial.print(staticBlinkerExample.blinkTime());
  Serial.println(" ms.");
}

void loop() {
  //run() needs to be called every loop for the blinker to run
  staticBlinkerExample.run();
  //StaticBLinker automatically casts to a boolean value, so it 
  //can be passed as true/false to a function
  updateLED(staticBlinkerExample);
}

void updateLED(bool onOrOff) {
  if (onOrOff == true) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
