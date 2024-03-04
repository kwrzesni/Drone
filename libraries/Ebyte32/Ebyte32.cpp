#include "avr/pgmspace.h"
#include "Ebyte32.h"

Ebyte32::Ebyte32(HardwareSerial* serial, int aux_pin, int baudRate)
  : serial{serial}, aux_pin{aux_pin}, baudRate{baudRate}
{
}

void Ebyte32::begin()
{
  serial->setTimeout(10);
  serial->begin(baudRate, SERIAL_8N1);
  pinMode(aux_pin, INPUT);
  while (!*serial);
  while (!serial->availableForWrite());
  delay(1000);
  unsigned char buff;
  serial->readBytes(&buff, 1);
}