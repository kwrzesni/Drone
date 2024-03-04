#include "Gyroscope.h"
#include <Wire.h>

Gyroscope::Gyroscope(float rollRateCalibration, float pitchRateCalibration, float yawRateCalibration)
  : CALIBRATION{rollRateCalibration, pitchRateCalibration, yawRateCalibration}
{}

void Gyroscope::begin() const
{
  //set full-scale to 500 dps and ODR to 416 Hz (high performance)
  writeRegister(CTRL2_G_ADDRESS, 0x64);
  // set high performance power mode
  writeRegister(CTRL7_G_ADDRESS, 0x00);
}

Gyroscope::Data Gyroscope::read() const
{
  readRegister(DATA_ADDRESS);
  Wire.requestFrom(I2Caddress, 6);
  const int16_t gyroX = Wire.read() | Wire.read() << 8;
  const int16_t gyroY = Wire.read() | Wire.read() << 8;
  const int16_t gyroZ = Wire.read() | Wire.read() << 8;
  return {gyroX * 500.0 / 32768 + CALIBRATION.rollRate, gyroY * 500.0 / 32768.0 + CALIBRATION.pitchRate, gyroZ * 500.0 / 32768.0 + CALIBRATION.yawRate};
}

void Gyroscope::writeRegister(int address, int value) const
{
  Wire.beginTransmission(I2Caddress);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission();
}

void Gyroscope::readRegister(int address) const
{
  Wire.beginTransmission(I2Caddress);
  Wire.write(address);
  Wire.endTransmission();
}