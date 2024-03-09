#include "Accelerometer.h"
#include <Wire.h>

Accelerometer::Accelerometer(const Data& accelerometerOffset, const AccelerometerMapping& accelerometeMapping)
  : ACCELEROMETER_OFFSET(accelerometerOffset), ACCELEROMETER_MAPPING(accelerometeMapping)
{}

void Accelerometer::begin() const
{
  //set full-scale to 8g and ODR to 1.66 kHz (high performance)
  writeRegister(CTRL1_XL_ADDRESS, 0x8C);
  // set high performance power mode
  writeRegister(CTRL6_C_ADDRESS, 0x00);
}

Accelerometer::Data Accelerometer::readUncalibrated() const
{
  readRegister(DATA_ADDRESS);
  Wire.requestFrom(I2Caddress, 6);
  const int16_t accX = Wire.read() | Wire.read() << 8;
  const int16_t accY = Wire.read() | Wire.read() << 8;
  const int16_t accZ = Wire.read() | Wire.read() << 8;
  return {accX / 4096.0, accY / 4096.0, accZ / 4096.0};
}

Accelerometer::Data Accelerometer::read() const
{
  Data temp = readUncalibrated();
  temp.x -= ACCELEROMETER_OFFSET.x;
  temp.y -= ACCELEROMETER_OFFSET.y;
  temp.z -= ACCELEROMETER_OFFSET.z;

  return {
            ACCELEROMETER_MAPPING.a11 * temp.x + ACCELEROMETER_MAPPING.a12 * temp.y + ACCELEROMETER_MAPPING.a13 * temp.z,
            ACCELEROMETER_MAPPING.a21 * temp.x + ACCELEROMETER_MAPPING.a22 * temp.y + ACCELEROMETER_MAPPING.a23 * temp.z,
            ACCELEROMETER_MAPPING.a31 * temp.x + ACCELEROMETER_MAPPING.a32 * temp.y + ACCELEROMETER_MAPPING.a33 * temp.z
         };
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