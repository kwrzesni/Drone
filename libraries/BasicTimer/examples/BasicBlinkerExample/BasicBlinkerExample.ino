#include <BasicTimer.h>

//The Blinkers are useful for blinking LEDs or anything else that you want
//turned on and off in rhythm using true/false values 

//Create a basic blinker.  The blink time can optionally be supplied here.
//It defaults to 500ms if now blink time is supplied at construction
BasicBlinker basicBlinkerExample(500);

void setup() {
  Serial.begin(9600);

  //We can set the time of a BasicBlinker programattically
  basicBlinkerExample.setBlinkTime(1000);
  //Resets the blinker (resets its timer and starts it at false)
  //This isn't necessary, but could be useful to roughly sync the timing
  //of multiple blinkers
  basicBlinkerExample.reset();

  //If we want the blinker to start on/true instead of off/false we 
  //can assign it's value directly.  It will toggle every time
  //it's internal timer expires
  basicBlinkerExample = true;

  Serial.print("We have create a blinker with blink time: ");
  Serial.print(basicBlinkerExample.blinkTime());
  Serial.println(" ms.");
}

void loop() {
  //run() needs to be called every loop for the blinker to run
  basicBlinkerExample.run();
  //BasicBlinker automatically casts to a boolean value, so it 
  //can be passed as true/false to a function
  updateLED(basicBlinkerExample);
}

void updateLED(bool onOrOff) {
  if (onOrOff == true) {
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
