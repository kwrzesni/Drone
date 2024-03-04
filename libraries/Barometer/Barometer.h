#ifndef BMP_388
#define BMP_388
#include <Arduino.h>

class Barometer
{
public:
  Barometer() = default;
  void begin();
  float readTemperature();
  float readPressure();
  float readAltitude();
private:
  void writeRegister(int address, int value) const;
  void readRegister(int address) const;
  bool reset();
  uint8_t readByte(uint8_t address);
  void readBytes(uint8_t address, uint8_t* data, uint16_t count);
  bool dataReady();
  bool getMeasurements(volatile float& temperature, volatile float& pressure, volatile float& altitude);
  bool getTemperatureAndPressure(volatile float &temperature, volatile float& pressure);
  float compensateTemperature(float uncompensatedTemperature);																	
	float compensatePressure(float uncompensatedPressure, float temperature);
private:
  static constexpr int I2Caddress = 0x76;
  static constexpr int PWR_CTRL_ADDRESS = 0x1B;
  static constexpr int OSR_ADDRESS = 0x1C;
  static constexpr int ODR_ADDRESS = 0x1D;
  static constexpr int CONFIG_ADDRESS = 0x1F;
  static constexpr int IF_CONF_ADDRESS = 0x1A;
  static constexpr int INT_STATUS_ADDRESS = 0x11;
  static constexpr int DATA_ADDRESS = 0x04;
  static constexpr int CMD_ADDRESS = 0x7E;
  static constexpr int EVENT_ADDRESS = 0x10;
  static constexpr int TRIM_PARAMS_ADDRESS = 0x31;
  static constexpr float SEA_LEVEL_PRESSURE = 1013.23f;
	struct Parameters
  {
		float T1;
		float T2;
		float T3;
		float P1;
		float P2;
		float P3;
		float P4;
		float P5;
		float P6;
		float P7;
		float P8;
		float P9;
		float P10;
		float P11;
	} parameters;
  float temperature;
  float pressure;
  float altitude;
};

#endif
