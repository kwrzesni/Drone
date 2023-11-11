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
public:
  Accelerometer() = default;
  void begin() const;
  Data read() const;
private:
  void writeRegister(int address, int value) const;
  void readRegister(int address) const;
private:
  static constexpr int I2Caddress = 0x6A;
  static constexpr int CTRL1_XL_ADDRESS = 0x10;
  static constexpr int CTRL6_C_ADDRESS = 0x15;
  static constexpr int DATA_ADDRESS = 0x28;
  static constexpr Data CALIBRATION = {-0.01f, 0.0f, 0.01f};
};

#endif
