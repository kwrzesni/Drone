#include "Accelerometer.h"
#include <Wire.h>

void Accelerometer::begin() const
{
  //set full-scale to 8g and ODR to 416 Hz (high performance)
  writeRegister(CTRL1_XL_ADDRESS, 0x6C);
  // set high performance power mode
  writeRegister(CTRL6_C_ADDRESS, 0x00);
}

Accelerometer::Data Accelerometer::read() const
{
  readRegister(DATA_ADDRESS);
  Wire.requestFrom(I2Caddress, 6);
  const int16_t accX = Wire.read() | Wire.read() << 8;
  const int16_t accY = Wire.read() | Wire.read() << 8;
  const int16_t accZ = Wire.read() | Wire.read() << 8;
  return {-(accX / 4096.0 + CALIBRATION.x), accY / 4096.0 + CALIBRATION.y, -(accZ / 4096.0 + CALIBRATION.z)};
}

void Accelerometer::writeRegister(int address, int value) const
{
  Wire.beginTransmission(I2Caddress);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission();
}

void Accelerometer::readRegister(int address) const
{
  Wire.beginTransmission(I2Caddress);
  Wire.write(address);
  Wire.endTransmission();
}