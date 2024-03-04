#include "Barometer.h"
#include <Wire.h>

struct IntegerParameters
{
	uint16_t T1;
	uint16_t T2;
	int8_t   T3;
	int16_t  P1;
  int16_t  P2;
	int8_t   P3;
	int8_t   P4;
	uint16_t P5;
	uint16_t P6;
	int8_t   P7;
	int8_t   P8;
	int16_t  P9;
	int8_t 	 P10;
	int8_t 	 P11;
} __attribute__ ((packed));

void Barometer::begin()
{
  reset();
	
  // read parameters
  IntegerParameters integergPrameters;
  readBytes(TRIM_PARAMS_ADDRESS, (uint8_t*)&integergPrameters, sizeof(IntegerParameters));
	parameters.T1 = (float)integergPrameters.T1 / powf(2.0f, -8.0f);
	parameters.T2 = (float)integergPrameters.T2 / powf(2.0f, 30.0f);
	parameters.T3 = (float)integergPrameters.T3 / powf(2.0f, 48.0f);
	parameters.P1 = ((float)integergPrameters.P1 - powf(2.0f, 14.0f)) / powf(2.0f, 20.0f);
	parameters.P2 = ((float)integergPrameters.P2 - powf(2.0f, 14.0f)) / powf(2.0f, 29.0f);
	parameters.P3 = (float)integergPrameters.P3 / powf(2.0f, 32.0f);
	parameters.P4 = (float)integergPrameters.P4 / powf(2.0f, 37.0f);
	parameters.P5 = (float)integergPrameters.P5 / powf(2.0f, -3.0f);
	parameters.P6 = (float)integergPrameters.P6 / powf(2.0f, 6.0f);
	parameters.P7 = (float)integergPrameters.P7 / powf(2.0f, 8.0f);
	parameters.P8 = (float)integergPrameters.P8 / powf(2.0f, 15.0f);
	parameters.P9 = (float)integergPrameters.P9 / powf(2.0f, 48.0f);
	parameters.P10 = (float)integergPrameters.P10 / powf(2.0f, 48.0f);
	parameters.P11 = (float)integergPrameters.P11 / powf(2.0f, 65.0f);

  // set iir filter coefficient to 3
  writeRegister(CONFIG_ADDRESS, 0x04);

  // set temperature oversampling to x1 and pressure oversampling to x8
  writeRegister(OSR_ADDRESS, 0x03);

  // set ODR to 50Hz
  writeRegister(ODR_ADDRESS, 0x02);

  // set normal mode and enable temperatue and pressure reading 
  writeRegister(PWR_CTRL_ADDRESS, 0x33);

  while(!getMeasurements(temperature, pressure, altitude));
}

float Barometer::readTemperature()
{
  getMeasurements(temperature, pressure, altitude);
  return temperature;
}

float Barometer::readPressure()
{
  getMeasurements(temperature, pressure, altitude);
  return pressure;
}

float Barometer::readAltitude()
{
  getMeasurements(temperature, pressure, altitude);
  return altitude;
}

void Barometer::writeRegister(int address, int value) const
{
  Wire.beginTransmission(I2Caddress);
  Wire.write(address);
  Wire.write(value);
  Wire.endTransmission();
}

void Barometer::readRegister(int address) const
{
  Wire.beginTransmission(I2Caddress);
  Wire.write(address);
  Wire.endTransmission();
}

bool Barometer::reset()
{
	writeRegister(CMD_ADDRESS, 0xB6);							
  delay(10);
	return readByte(EVENT_ADDRESS) & 1;																
}

uint8_t Barometer::readByte(uint8_t address)
{
  Wire.beginTransmission(I2Caddress);         
	Wire.write(address);                  
	Wire.endTransmission(false);             
	Wire.requestFrom(I2Caddress, (uint8_t)1);	 
	return Wire.read();  
}

void Barometer::readBytes(uint8_t address, uint8_t* data, uint16_t count)
{
  Wire.beginTransmission(I2Caddress);          
	Wire.write(address);                   
	Wire.endTransmission(false);
	Wire.requestFrom(I2Caddress, (uint8_t)count);  
  for (uint8_t i = 0; Wire.available(); ++i)
  {
    data[i] = Wire.read(); 
  }
}

bool Barometer::dataReady()
{
	if (readByte(INT_STATUS_ADDRESS) & 8)
	{
		return true;
	}
	return false;
}

bool Barometer::getMeasurements(volatile float &temperature, volatile float &pressure, volatile float &altitude)
{
  if (getTemperatureAndPressure(temperature, pressure))
	{
		altitude = ((float)powf(SEA_LEVEL_PRESSURE / pressure, 0.190223f) - 1.0f) * (temperature + 273.15f) * 100.0f / 0.0065f;
		return true;
	}
	return false;
}

bool Barometer::getTemperatureAndPressure(volatile float& temperature, volatile float& pressure)	
{
	if (!dataReady())
	{	
		return false;
	}
	uint8_t data[6];
	readBytes(DATA_ADDRESS, &data[0], 6);
	int32_t adcTemp = (int32_t)data[5] << 16 | (int32_t)data[4] << 8 | (int32_t)data[3];
	int32_t adcPres = (int32_t)data[2] << 16 | (int32_t)data[1] << 8 | (int32_t)data[0];
	temperature = compensateTemperature((float)adcTemp);
	pressure = compensatePressure((float)adcPres, temperature);
	pressure /= 100.0f;
	return true;
}

float Barometer::compensateTemperature(float uncompensatedTemperature)
{
	float partial_data1 = uncompensatedTemperature - parameters.T1;
	float partial_data2 = partial_data1 * parameters.T2;
	return partial_data2 + partial_data1 * partial_data1 * parameters.T3;
}	

float Barometer::compensatePressure(float uncompensatedPressure, float temperature)
{
	float partial_data1 = parameters.P6 * temperature;
	float partial_data2 = parameters.P7 * temperature * temperature;
	float partial_data3 = parameters.P8 * temperature * temperature * temperature;
	float partial_out1 = parameters.P5 + partial_data1 + partial_data2 + partial_data3;
	partial_data1 = parameters.P2 * temperature;
	partial_data2 = parameters.P3 * temperature * temperature;
	partial_data3 = parameters.P4 * temperature * temperature * temperature;
	float partial_out2 = uncompensatedPressure * (parameters.P1 +
	partial_data1 + partial_data2 + partial_data3);
	partial_data1 = uncompensatedPressure * uncompensatedPressure;
	partial_data2 = parameters.P9 + parameters.P10 * temperature;
	partial_data3 = partial_data1 * partial_data2;
	float partial_data4 = partial_data3 + uncompensatedPressure * uncompensatedPressure * uncompensatedPressure * parameters.P11;
	return partial_out1 + partial_out2 + partial_data4;
}