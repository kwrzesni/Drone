#include "Gyroscope.h"
#include <Wire.h>

void Gyroscope::begin() const
{
  //set full-scale to 500 dps and ODR to 416 Hz (high performance)
  writeRegister(CTRL2_G_ADDRESS, 0x64);
  // set high performance power mode
  writeRegister(CTRL7_G_ADDRESS, 0x00);
}

void Gyroscope::init()
{
  float rollRatesSum = 0.0f;
  float pitchRatesSum = 0.0f;
  float yawRatesSum = 0.0f;
  for (int i = 0; i < N_INIT_POINTS; ++i)
  {
	 Data data = read();
	 rollRatesSum += data.rollRate;
	 pitchRatesSum += data.pitchRate;
	 yawRatesSum += data.yawRate;
	 delay(4);
  }
  calibration.rollRate = rollRatesSum / N_INIT_POINTS;
  calibration.pitchRate = pitchRatesSum / N_INIT_POINTS;
  calibration.yawRate = yawRatesSum / N_INIT_POINTS;
}

Gyroscope::Data Gyroscope::read() const
{
  readRegister(DATA_ADDRESS);
  Wire.requestFrom(I2Caddress, 6);
  const int16_t gyroX = Wire.read() | Wire.read() << 8;
  const int16_t gyroY = Wire.read() | Wire.read() << 8;
  const int16_t gyroZ = Wire.read() | Wire.read() << 8;
  return {gyroX * 500.0 / 32768 - calibration.rollRate, gyroY * 500.0 / 32768.0 - calibration.pitchRate, gyroZ * 500.0 / 32768.0 - calibration.yawRate};
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