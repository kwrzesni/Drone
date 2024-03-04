#ifndef GYROSCOPE
#define GYROSCOPE

class Gyroscope
{
public:
  struct Data
  {
    float rollRate;
    float pitchRate;
    float yawRate;
  };
public:
  Gyroscope(float rollRateCalibration = 0.0f, float pitchRateCalibration = 0.0f, float yawRateCalibration = 0.0f);
  void begin() const;
  Data read() const;
private:
  void writeRegister(int address, int value) const;
  void readRegister(int address) const;
private:
  static constexpr int I2Caddress = 0x6A;
  static constexpr int CTRL2_G_ADDRESS = 0x11;
  static constexpr int CTRL7_G_ADDRESS = 0x16;
  static constexpr int DATA_ADDRESS = 0x22;
  const Data CALIBRATION;
};

#endif
