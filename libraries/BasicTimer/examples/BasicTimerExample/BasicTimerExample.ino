#include <BasicTimer.h>

// Variable that stores our LED state
bool ledState = LOW;
// Constructs a default basic timer (the default timeout is 500ms)
BasicTimer defaultExample;
// Constructs a basic timer
BasicTimer basicExample;
// Constructs a timer where the timeout is supplied at construction (30 seconds)
BasicTimer constructedTimerExample(10000);
// Constructs a timer with a fixed timeout that can never change (this saves 4 bytes of memory)
StaticTimer<5200> staticExample;

void send_hello() {
  Serial.print(F("Hello :) basicExample's timeout is: "));
  Serial.println(basicExample.timeout());
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // Prepares/resets the defaultExample timer 
  defaultExample.begin();
  // Resets the basicExample timer and starts it with a timeout of 1000ms
  basicExample.begin(1000);
  // Prepares/resets the constructedTimerExample timer 
  constructedTimerExample.begin();
  // Calling now() on any timer gets the current elapsed time in milliseconds since the program started
  Serial.print(F("Timer Example started - The current elapsed time is: "));
  Serial.println(defaultExample.now());
}

void loop() {
  // Simple use, blinks the LED every 500 milliseconds with defaultExample;
  if (defaultExample.hasExpired()) {
    ledState = !ledState;
    digitalWrite(LED_BUILTIN, ledState);
    defaultExample.reset();
  }

  // Simple use, basicExample says hello every time it expires
  if (basicExample.hasExpired()) {
    send_hello();
    basicExample.reset();
  }

  //Fancy use, constructedTimerExample modifies the timer of basicExample each time it expires
  if (constructedTimerExample.hasExpired()) {
    unsigned long currentTimeoutValue = basicExample.timeout();
    unsigned long newTimeoutValue = currentTimeoutValue + 500;
    if (newTimeoutValue > 10000) { 
      basicExample.setTimeout(1000);
    } else {
      basicExample.setTimeout(newTimeoutValue);
    }
    constructedTimerExample.reset();
  }

  //staticExample Timer is used to print the elapsed time of defaultExample since it was last reset 
  if (staticExample.hasExpired()) {
    staticExample.reset();
    Serial.print(F("defaultExample elapsed: "));
    Serial.println(defaultExample.elapsedTime());
  }
}
