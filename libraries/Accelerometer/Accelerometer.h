#ifndef ACCELEROMETER
#define ACCELEROMETER

class Accelerometer
{
public:
  struct Data
  {
    float x;
    float y;
    float z;
  };
  struct AccelerometerMapping
  {
    float a11; float a12; float a13;
    float a21; float a22; float a23;
    float a31; float a32; float a33;
  };
public:
  Accelerometer(const Data& accelerometerOffset = {0, 0, 0}, const AccelerometerMapping& accelerometeMapping = {1, 0, 0, 0, 1, 0, 0, 0, 1});
  void begin() const;
  Data readUncalibrated() const;
  Data read() const;
private:
  void writeRegister(int address, int value) const;
  void readRegister(int address) const;
private:
  static constexpr int I2Caddress = 0x6A;
  static constexpr int CTRL1_XL_ADDRESS = 0x10;
  static constexpr int CTRL6_C_ADDRESS = 0x15;
  static constexpr int DATA_ADDRESS = 0x28;
  const Data ACCELEROMETER_OFFSET;
  const AccelerometerMapping ACCELEROMETER_MAPPING;
};

#endif
