#include <BasicTimer.h>

// Storage Variable for our blinking LED
bool ledState = LOW;

// Forward declration of our callback functions
void on_timeout_1();
void on_timeout_2();

// Construct our first example timer, constructed with default values
CallbackTimer exampleTimer1;

// Construct our second example timer, setting the timeout to 1000ms, 
// the callback function to on_timeout_2(), and the mode to continuous
// at construction. 
// 
// Timers can have two modes:  
//
//   TIMER_RUN_MODE_ONE_SHOT is the default.  Once started, the timer will 
//   run and the first time it expires the callback will be executed.  
//   It will never reset.
//   
//   TIMER_RUN_MODE_CONTINUOUS is also an option.  Once started. the timer
//   will run and each time it expires, it will reset itself and the callback
//   will be executed each time (in this case once every 250ms)
//
CallbackTimer exampleTimer2(250, on_timeout_2, TIMER_RUN_MODE_CONTINUOUS);


void setup() {
  //Prepare the LED for blinking
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  //Programmatically sets the callback function for the timer.  Setting
  //to nullptr or NULL will disable the callback funcion.
  exampleTimer1.onExpire(on_timeout_1);

  //We are going to use exampleTimer1, to disable and enable exampleTimer2
  //which controls a blinking LED.  After 10 seconds I want exampleTimer2 to
  //execute it's callback the first time.
  exampleTimer1.setTimeout(10000);
  exampleTimer1.setMode(TIMER_RUN_MODE_ONE_SHOT);
  //This could also be accomplished with one function, prepare, which
  //takes the timeout in milliseconds and the mode as arguments
  exampleTimer1.prepare(10000, TIMER_RUN_MODE_ONE_SHOT);
  //We can also prepare and start the timer in one go with begin,
  //which takes the same arguments as prepare but starts the timer at the
  //same time
  exampleTimer1.begin(10000, TIMER_RUN_MODE_ONE_SHOT);
  //Use begin() or start() to start a timer.
  exampleTimer2.start();
}

void loop() {
  //Call run for each timer in the loop to have the timers run
  exampleTimer1.run();
  exampleTimer2.run();
}

void on_timeout_1() {
//If exampleTimer2 is running, I want to turn it off 
//for 5 seconds.  After it has been off for 5 seconds,
//I want to turn it back on for 10 seconds, etc...
  if (exampleTimer2.hasStarted()) {
    //Stop the blinking timer
    exampleTimer2.stop();
    //We want the LED to be off, in case it stops blinking while on
    ledState = LOW;
    digitalWrite(LED_BUILTIN, ledState);
    //Reset the timer to run this function again after 5 seconds
    exampleTimer1.setTimeout(5000);
    exampleTimer1.start();
  } else {
    //We want the LED to be on when it starts blinking
    ledState = HIGH;
    digitalWrite(LED_BUILTIN, ledState);
    //Start the blinking timer
    exampleTimer2.start();
    //Reset the timer to run this function again after 10 seconds
    exampleTimer1.setTimeout(10000);
    exampleTimer1.start();
  }
}

void on_timeout_2() {
  //Blinks the LED on and off
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
}