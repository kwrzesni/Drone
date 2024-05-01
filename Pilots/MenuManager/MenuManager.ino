#include <Adafruit_GFX.h>
#include <ILI9488.h>
#include "MenuManager.h"
#include <XPT2046_Touchscreen.h>

// VCC   - 3.3
// GND   - G
// CS    - G
// RESET - A3
// D/C   - A1
// SDI   - A7
// SCK   - A5
// LED   - 3.3

ILI9488 tft(PA0, PA1, PA3);
MenuManager menuManager(tft);
XPT2046_Touchscreen ts(PB12);

void setup() 
{
  pinMode(PB12, OUTPUT);
  pinMode(PA0, OUTPUT);
  digitalWrite(PB12, HIGH);
  digitalWrite(PA0, HIGH);

  Serial.begin(9600);
  Serial.setTimeout(10);
  menuManager.begin();
  //ts.begin();
  //ts.setRotation(3);
}

void loop() 
{
  if (Serial.available())
  {
    String str = Serial.readString();
    for (char ch : str)
    {
      switch (ch)
      {
        case 'L':
          menuManager.update(MenuControlCommand::left);
          break;
        case 'R':
          menuManager.update(MenuControlCommand::right);
          break;
        case 'U':
          menuManager.update(MenuControlCommand::up);
          break;
        case 'D':
          menuManager.update(MenuControlCommand::down);
          break;
        case 'O':
          menuManager.update(MenuControlCommand::ok);
          break;
        case 'B':
          menuManager.update(MenuControlCommand::back);
          break;
        case 'C':
          break;
      }
    }
  }
  /*if (ts.touched())
  {
    TS_Point p = ts.getPoint();
    Serial.print("x = "); Serial.print(p.x); Serial.print(", y = "); Serial.print(p.y); Serial.print(", z = "); Serial.println(p.z);
  }*/
}
