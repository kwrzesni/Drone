#include "Magnetometer.h"
#include <Wire.h>

Magnetometer::Magnetometer(const Data& magneticOffset, const MagneticMapping& magneticMapping)
	: MAGNETIC_OFFSET(magneticOffset), MAGNETIC_MAPPING(magneticMapping)
{}

void Magnetometer::begin() const
{
  // set (ultra-high-performance mode for X and Y); (155 Hz ODR)
  writeRegister(CTRL_REG1_ADDRESS, 0x62);

  // set FS = 00 (+/- 4 gauss full scale)
  writeRegister(CTRL_REG2_ADDRESS, 0x00);

  // set MD = 00 (continuous-conversion mode)
  writeRegister(CTRL_REG3_ADDRESS, 0x00);

  // set OMZ = 11 (ultra-high-performance mode for Z)
  writeRegister(CTRL_REG4_ADDRESS, 0x0C);

  // set BDU = 1 (block data update)
  writeRegister(CTRL_REG5_ADDRESS, 0x40);
}

Magnetometer::Data Magnetometer::readUncalibrated() const
{
  readRegister(STATUS_REG_ADDRESS);
  Wire.requestFrom(I2Caddress, 1);
  Wire.read();

  readRegister(DATA_ADDRESS);
  Wire.requestFrom(I2Caddress, 6);
  uint8_t xlm = Wire.read();
  uint8_t xhm = Wire.read();
  uint8_t ylm = Wire.read();
  uint8_t yhm = Wire.read();
  uint8_t zlm = Wire.read();
  uint8_t zhm = Wire.read();

  return {-(((int16_t)(xhm << 8 | xlm))) * 400 / 32768.0f, (((int16_t)(yhm << 8 | ylm))) * 400 / 32768.0f, -(((int16_t)(zhm << 8 | zlm))) * 400 / 32768.0f };
}

Magnetometer::Data Magnetometer::read() const
{
  Data temp = readUncalibrated();
  temp.x -= MAGNETIC_OFFSET.x;
  temp.y -= MAGNETIC_OFFSET.y;
  temp.z -= MAGNETIC_OFFSET.z;

  return {
            MAGNETIC_MAPPING.a11 * temp.x + MAGNETIC_MAPPING.a12 * temp.y + MAGNETIC_MAPPING.a13 * temp.z,
            MAGNETIC_MAPPING.a21 * temp.x + MAGNETIC_MAPPING.a22 * temp.y + MAGNETIC_MAPPING.a23 * temp.z,
            MAGNETIC_MAPPING.a31 * temp.x + MAGNETIC_MAPPING.a32 * temp.y + MAGNETIC_MAPPING.a33 * temp.z
         };
}

void Magnetometer::writeRegister(int address, int value) const
{
  Wire.beginTransmission(I2Caddress);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission();
}

void Magnetometer::readRegister(int address) const
{
  Wire.beginTransmission(I2Caddress);
  Wire.write(address);
  Wire.endTransmission();
}