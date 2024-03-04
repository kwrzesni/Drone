#ifndef EBYTE_32
#include "Arduino.h"

class ConfigurableEbyte32
{
public:
  enum class Mode : unsigned char
  {
    normal = 0,
    wakeUp = 1,
    power_saving = 2,
    sleep = 3
  };
  
  class Configuration
  {
    friend ConfigurableEbyte32;
  public:
    enum class UARTParityBit : unsigned char
    {
      v8N1 = 0,
      v8O1 = 1,
      v8E1 = 2
    };
    enum class TTLUARTBaudRate : unsigned char
    {
      v1200 = 0,
      v2400 = 1,
      v4800 = 2,
      v9600 = 3,
      v19200 = 4,
      v38400 = 5,
      v57600 = 6,
      v115200 = 7
    };
    enum class AirDataRate : unsigned char
    {
      v0_3k = 0,
      v1_2k = 1,
      v2_4k = 2,
      v4_8k = 3,
      v9_6k = 4,
      v19_2k = 5
    };
    enum class TransmissionMode : unsigned char
    {
      transparent = 0,
      fixed = 1
    };
    enum class WirelessWakeUpTime : unsigned char
    {
      v250 = 0,
      v500 = 1,
      v750 = 2,
      v1000 = 3,
      v1250 = 4,
      v1500 = 5,
      v1750 = 6,
      v2000 = 7
    };
    enum class IODriveMode : unsigned char
    {
      open_collector = 0,
      push_pull = 1
    };
    enum class FEC : unsigned char
    {
      off = 0,
      on = 1
    };
    enum class TransmissionPower : unsigned char
    {
      v30dBm = 0,
      v27dBm = 1,
      v24dBm = 2,
      v21dBm = 3
    };
  private:
    Configuration() = default;
    void readConfiguration(HardwareSerial* serial);
  public:
    String toString() const;
    unsigned short getAddress() const;
    unsigned short setAddress(unsigned short address);
    UARTParityBit getUartParityBit() const;
    UARTParityBit setUartParityBit(UARTParityBit uartParityBit);
    TTLUARTBaudRate getTtlUARTBaudRate() const;
    TTLUARTBaudRate setTtlUARTBaudRate(TTLUARTBaudRate ttlUARTBaudRate);
    AirDataRate getAirDataRate() const;
    AirDataRate setAirDataRate(AirDataRate airDataRate);
    unsigned char getChannel() const;
    unsigned char setChannel(unsigned char channel);
    TransmissionMode getTransmissionMode() const;
    TransmissionMode setTransmissionMode(TransmissionMode transmissionMode);
    IODriveMode getIoDriveMode() const;
    IODriveMode setIoDriveMode(IODriveMode ioDriveMode);
    WirelessWakeUpTime getWirelessWakeUpTime() const;
    WirelessWakeUpTime setWirelessWakeUpTime(WirelessWakeUpTime wirelessWakeUpTime);
    FEC getFec() const;
    FEC setFec(FEC fec);
    TransmissionPower getTransmissionPower() const;
    TransmissionPower setTransmissionPower(TransmissionPower transmissionPower);
  private:
    static constexpr int N_CONFIGURATION_BYTES = 6;
    using ConfigurationBytes = unsigned char[N_CONFIGURATION_BYTES];

    ConfigurationBytes configurationBytes;
    unsigned short address;
    UARTParityBit uartParityBit;
    TTLUARTBaudRate ttlUARTBaudRate;
    AirDataRate airDataRate;
    unsigned char channel;
    TransmissionMode transmissionMode;
    IODriveMode ioDriveMode;
    WirelessWakeUpTime wirelessWakeUpTime;
    FEC fec;
    TransmissionPower transmissionPower;
  };
public:
  ConfigurableEbyte32(HardwareSerial* serial, int aux_pin, int m0_pin, int m1_pin, int baudRate=9600, Mode mode=Mode::normal);
  void begin();
  Mode getMode() const;
  void setMode(Mode newMode);
  const Configuration& getConfiguration();
  String getConfigurationString();
  void setConfiguration(const Configuration& newConfiguration, bool permanent=true);
  void reset();
  void send(unsigned short address, unsigned char channel, const unsigned char* buffer, size_t size);
  void receive(unsigned char* buffer, size_t size);
private:
  void resetInSleepMode();
  void readConfiguration();
  void readConfigurationInSleepMode();
  void setConfigurationInSleepMode(bool permanent);
  void setPermanenttConfigurationInSleepMode();
  void setNonPermanenttConfigurationInSleepMode();
  String parseConfigurationToString() const;
  void sendReadConfigurationCommand();
  inline void waitForLowAux() const;
  inline void waitForHighAux() const;
  void writeInNormalMode(const unsigned char* buffer, size_t size);
  void readInNormalMode(unsigned char* buffer, size_t size);
private:
  HardwareSerial* serial;
  const int aux_pin;
  const int m0_pin;
  const int m1_pin;
  int baudRate;
  Mode mode;
  bool isConfigurationRead = false;
  Configuration configuration;
};

#endif
