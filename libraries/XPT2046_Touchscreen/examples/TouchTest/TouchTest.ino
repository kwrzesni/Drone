#include <XPT2046_Touchscreen.h>
#include <SPI.h>

#define CS_PIN  PB12
// MOSI=11, MISO=12, SCK=13

//XPT2046_Touchscreen ts(CS_PIN);
#define TIRQ_PIN  2
//XPT2046_Touchscreen ts(CS_PIN);  // Param 2 - NULL - No interrupts
//XPT2046_Touchscreen ts(CS_PIN, 255);  // Param 2 - 255 - No interrupts
XPT2046_Touchscreen ts(CS_PIN, PA0);  // Param 2 - Touch IRQ Pin - interrupt enabled polling

int mapX(int x)
{
  static const int a = 274;
  static const int b = 3999;
  return (480 * (x - a)) / (b - a);
}

int mapY(int y)
{
  static const int a = 341;
  static const int b = 4009;
  return (320 * (y - a)) / (b - a);
}

void setup() {
  Serial.begin(38400);
  ts.begin();
  ts.setRotation(3);
  while (!Serial && (millis() <= 1000));
}

int minx = 10000; int maxx = -10000; int miny = 10000; int maxy = -10000;

void loop() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    if (p.x < minx) minx = p.x;
    if (p.x > maxx) maxx = p.x;
    if (p.y < miny) miny = p.y;
    if (p.y > maxy) maxy = p.y;

    /*Serial.print(minx); Serial.print(", ");
    Serial.print(maxx); Serial.print(", ");
    Serial.print(miny); Serial.print(", ");
    Serial.println(maxy);*/
    Serial.print(mapX(p.x)); Serial.print(", ");
    Serial.println(mapY(p.y));
  }
}

