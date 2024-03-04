#ifndef MAGNETOMETER
#define MAGNETOMETER

class Magnetometer
{
public:
  struct Data
  {
    float x;
    float y;
    float z;
  };
  struct MagneticMapping
  {
    float a11; float a12; float a13;
    float a21; float a22; float a23;
    float a31; float a32; float a33;
  };
public:
  Magnetometer(const Data& magneticOffset = {0, 0, 0}, const MagneticMapping& magneticMapping = {1, 0, 0, 0, 1, 0, 0, 0, 1});
  void begin() const;
  Data readUncalibrated() const;
  Data read() const;
private:
  void writeRegister(int address, int value) const;
  void readRegister(int address) const;
private:
  static constexpr int I2Caddress = 0x1C;
  static constexpr int CTRL_REG1_ADDRESS = 0x20;
  static constexpr int CTRL_REG2_ADDRESS = 0x21;
  static constexpr int CTRL_REG3_ADDRESS = 0x22;
  static constexpr int CTRL_REG4_ADDRESS = 0x23;
  static constexpr int CTRL_REG5_ADDRESS = 0x24;
  static constexpr int STATUS_REG_ADDRESS = 0x27;
  static constexpr int DATA_ADDRESS = 0x28;
  const Data MAGNETIC_OFFSET;
  const MagneticMapping MAGNETIC_MAPPING;
};

#endif
