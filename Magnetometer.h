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
public:
  Magnetometer() = default;
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
  static constexpr Data MAGNETIC_OFFSET = {-4.293401f, -29.466995f, 1.294055f};
  static constexpr float MAGNETIC_MAPPING[3][3] = {{0.790252f, -0.008337f, 0.020657f}, {-0.008337f, 0.831763f, 0.010408f}, {0.020657f, 0.010408f, 0.815301f}};
};

#endif
