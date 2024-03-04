#include "Button.h"

Button::Button(uint8 pin)
  : pin{pin}
{}

void Button::begin()
{
  pinMode(pin, INPUT_PULLDOWN);
}
  
uint8 Button::read()
{
  uint8 reading_pushbutton = digitalRead(pin);

  if (reading_pushbutton != lastButtonState) 
  {
    lastDebouncingTime = millis();
  }

  if ((millis() - lastDebouncingTime) > DEBOUNCING_DELAY) 
  {
    if (buttonState != reading_pushbutton)
    {
      buttonState = reading_pushbutton;
    }
  }
  lastButtonState = reading_pushbutton;
  return buttonState;
}