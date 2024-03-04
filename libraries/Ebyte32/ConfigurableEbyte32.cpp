#include "avr/pgmspace.h"
#include "ConfigurableEbyte32.h"

String bytesToString(const unsigned char* bytes, unsigned length);
String addressToString(unsigned short address);
String uartParityBitToString(ConfigurableEbyte32::Configuration::UARTParityBit uartParityBit);
String ttlUARTBaudRateToString(ConfigurableEbyte32::Configuration::TTLUARTBaudRate ttlUARTBaudRate);
String airDataRateToString(ConfigurableEbyte32::Configuration::AirDataRate airDataRate);
String channelToString(unsigned char channel);
String transmissionModeToString(ConfigurableEbyte32::Configuration::TransmissionMode transmissionMode);
String ioDriveModeToString(ConfigurableEbyte32::Configuration::IODriveMode ioDriveMode);
String wirelessWakeUpTimeToString(ConfigurableEbyte32::Configuration::WirelessWakeUpTime wirelessWakeUpTime);
String fecToString(ConfigurableEbyte32::Configuration::FEC fec);
String transmissionPowerToString(ConfigurableEbyte32::Configuration::TransmissionPower transmissionPower);

void ConfigurableEbyte32::Configuration::readConfiguration(HardwareSerial* serial)
{
  serial->readBytes(configurationBytes, N_CONFIGURATION_BYTES);
  address = (configurationBytes[1] << 8) | configurationBytes[2];
  uartParityBit = static_cast<UARTParityBit>(configurationBytes[3] >> 6u);
  ttlUARTBaudRate = static_cast<TTLUARTBaudRate>((configurationBytes[3] >> 3u) & 7u);
  airDataRate = static_cast<AirDataRate>(configurationBytes[3] & 7u);
  channel = static_cast<unsigned char>(configurationBytes[4] & 31u);
  transmissionMode = static_cast<TransmissionMode>(configurationBytes[5] >> 7u);
  ioDriveMode = static_cast<IODriveMode>((configurationBytes[5] >> 6u) & 1u);
  wirelessWakeUpTime = static_cast<WirelessWakeUpTime>((configurationBytes[5] >> 3u) & 7u);
  fec = static_cast<FEC>((configurationBytes[5] >> 6u) & 1u);
  transmissionPower = static_cast<TransmissionPower>(configurationBytes[5] & 3u);
}

String ConfigurableEbyte32::Configuration::toString() const
{
  return "Configuration bytes: 0x" + bytesToString(configurationBytes, N_CONFIGURATION_BYTES) + "\n" +
         "Adrres: 0x" + addressToString(address) + "\n" +
         "UART parity bit: " + uartParityBitToString(uartParityBit) + "\n" +
         "TTL UART baud rate (bts): " + ttlUARTBaudRateToString(ttlUARTBaudRate) + "\n" +
         "Air data rate (bts): " + airDataRateToString(airDataRate) + "\n" +
         "Channel: " + channelToString(channel) + "\n" +
         "Transmission mode: " + transmissionModeToString(transmissionMode) + "\n" +
         "IO drive mode: " + ioDriveModeToString(ioDriveMode) + "\n" +
         "Wireless wake-up time: " + wirelessWakeUpTimeToString(wirelessWakeUpTime) + "\n" +
         "FEC: " + fecToString(fec) + "\n" +
         "Transmission power: " + transmissionPowerToString(transmissionPower) + "\n";
}

unsigned short ConfigurableEbyte32::Configuration::getAddress() const
{
  return address;
}

unsigned short ConfigurableEbyte32::Configuration::setAddress(unsigned short address)
{
  this->address = address;
  configurationBytes[1] = address >> 8u;
  configurationBytes[2] = address & 255u;
}

ConfigurableEbyte32::Configuration::UARTParityBit ConfigurableEbyte32::Configuration::getUartParityBit() const
{
  return uartParityBit;
}

ConfigurableEbyte32::Configuration::UARTParityBit ConfigurableEbyte32::Configuration::setUartParityBit(ConfigurableEbyte32::Configuration::UARTParityBit uartParityBit)
{
  this->uartParityBit = uartParityBit;
  configurationBytes[3] = (static_cast<unsigned char>(uartParityBit) << 6u) | (configurationBytes[3] & 63u);
}
    
ConfigurableEbyte32::Configuration::TTLUARTBaudRate ConfigurableEbyte32::Configuration::getTtlUARTBaudRate() const
{
  return ttlUARTBaudRate;
}
    
ConfigurableEbyte32::Configuration::TTLUARTBaudRate ConfigurableEbyte32::Configuration::setTtlUARTBaudRate(ConfigurableEbyte32::Configuration::TTLUARTBaudRate ttlUARTBaudRate)
{
  this->ttlUARTBaudRate = ttlUARTBaudRate;
  configurationBytes[3] = (static_cast<unsigned char>(ttlUARTBaudRate) << 3u) | (configurationBytes[3] & 199u);
}
    
ConfigurableEbyte32::Configuration::AirDataRate ConfigurableEbyte32::Configuration::getAirDataRate() const
{
  return airDataRate;
}
    
ConfigurableEbyte32::Configuration::AirDataRate ConfigurableEbyte32::Configuration::setAirDataRate(ConfigurableEbyte32::Configuration::AirDataRate airDataRate)
{
  this->airDataRate = airDataRate;
  configurationBytes[3] = static_cast<unsigned char>(airDataRate) | (configurationBytes[3] & 248u);
}
    
unsigned char ConfigurableEbyte32::Configuration::getChannel() const
{
  return channel;
}
    
unsigned char ConfigurableEbyte32::Configuration::setChannel(unsigned char channel)
{
  this->channel = channel;
  configurationBytes[4] = static_cast<unsigned char>(channel) | (configurationBytes[4] & 224u);
}
    
ConfigurableEbyte32::Configuration::TransmissionMode ConfigurableEbyte32::Configuration::getTransmissionMode() const
{
  return transmissionMode;
}
    
ConfigurableEbyte32::Configuration::TransmissionMode ConfigurableEbyte32::Configuration::setTransmissionMode(ConfigurableEbyte32::Configuration::TransmissionMode transmissionMode)
{
  this->transmissionMode = transmissionMode;
  configurationBytes[5] = (static_cast<unsigned char>(transmissionMode) << 7u) | (configurationBytes[5] & 127u);
}
    
ConfigurableEbyte32::Configuration::IODriveMode ConfigurableEbyte32::Configuration::getIoDriveMode() const
{
  return ioDriveMode;
}

ConfigurableEbyte32::Configuration::IODriveMode ConfigurableEbyte32::Configuration::setIoDriveMode(ConfigurableEbyte32::Configuration::IODriveMode ioDriveMode)
{
  this->ioDriveMode = ioDriveMode;
  configurationBytes[5] = (static_cast<unsigned char>(ioDriveMode) << 6u) | (configurationBytes[5] & 191u);
}

ConfigurableEbyte32::Configuration::WirelessWakeUpTime ConfigurableEbyte32::Configuration::getWirelessWakeUpTime() const
{
  return wirelessWakeUpTime;
}

ConfigurableEbyte32::Configuration::WirelessWakeUpTime ConfigurableEbyte32::Configuration::setWirelessWakeUpTime(ConfigurableEbyte32::Configuration::WirelessWakeUpTime wirelessWakeUpTime)
{
  this->wirelessWakeUpTime = wirelessWakeUpTime;
  configurationBytes[5] = (static_cast<unsigned char>(wirelessWakeUpTime) << 3u) | (configurationBytes[5] & 199u);
}

ConfigurableEbyte32::Configuration::FEC ConfigurableEbyte32::Configuration::getFec() const
{
  return fec;
}

ConfigurableEbyte32::Configuration::FEC ConfigurableEbyte32::Configuration::setFec(ConfigurableEbyte32::Configuration::FEC fec)
{
  this->fec = fec;
  configurationBytes[5] = (static_cast<unsigned char>(fec) << 2u) | (configurationBytes[5] & 251u);
}

ConfigurableEbyte32::Configuration::TransmissionPower ConfigurableEbyte32::Configuration::getTransmissionPower() const
{
  return transmissionPower;
}

ConfigurableEbyte32::Configuration::TransmissionPower ConfigurableEbyte32::Configuration::setTransmissionPower(ConfigurableEbyte32::Configuration::TransmissionPower transmissionPower)
{
  this->transmissionPower = transmissionPower;
  configurationBytes[5] = static_cast<unsigned char>(transmissionPower) | (configurationBytes[5] & 252u);
}

ConfigurableEbyte32::ConfigurableEbyte32(HardwareSerial* serial, int aux_pin, int m0_pin, int m1_pin, int baudRate, Mode mode)
  : serial{serial}, aux_pin{aux_pin}, m0_pin{m0_pin}, m1_pin{m1_pin}, baudRate{baudRate}, mode{mode}
{
}

void ConfigurableEbyte32::begin()
{
  serial->begin(baudRate, SERIAL_8N1);
  pinMode(aux_pin, INPUT);
  pinMode(m0_pin, OUTPUT);
  pinMode(m1_pin, OUTPUT);
  setMode(mode);
  while (!*serial);
  while (!serial->availableForWrite());
  waitForHighAux();
  delay(1000);
}

ConfigurableEbyte32::Mode ConfigurableEbyte32::getMode() const
{
  return mode;
}

void ConfigurableEbyte32::setMode(Mode newMode)
{
  mode = newMode;
  switch(mode)
  {
    case Mode::normal:
      digitalWrite(m0_pin, LOW);
      digitalWrite(m1_pin, LOW);
      break;
    case Mode::wakeUp:
      digitalWrite(m0_pin, HIGH);
      digitalWrite(m1_pin, LOW);
      break;
    case Mode::power_saving:
      digitalWrite(m0_pin, LOW);
      digitalWrite(m1_pin, HIGH);
      break;
    case Mode::sleep:
      digitalWrite(m0_pin, HIGH);
      digitalWrite(m1_pin, HIGH);
      break;
  }
  waitForHighAux();
  delay(100);
}

const ConfigurableEbyte32::Configuration& ConfigurableEbyte32::getConfiguration()
{
  if (!isConfigurationRead)
  {
    readConfiguration();
  }
  return configuration;
}

String ConfigurableEbyte32::getConfigurationString()
{
  if (!isConfigurationRead)
  {
    readConfiguration();
  }
  return parseConfigurationToString();
}

void ConfigurableEbyte32::setConfiguration(const Configuration& newConfiguration, bool permanent)
{
  configuration = newConfiguration;
  if (mode == Mode::sleep)
  {
    setConfigurationInSleepMode(permanent);
  }
  else
  {
    Mode currentMode = mode;
    setMode(Mode::sleep);
    setConfigurationInSleepMode(permanent);
    setMode(currentMode);
  }
}

void ConfigurableEbyte32::reset()
{
  if (mode == Mode::sleep)
  {
    resetInSleepMode();
  }
  else
  {
    Mode currentMode = mode;
    setMode(Mode::sleep);
    resetInSleepMode();
    setMode(currentMode);
  }
}

void ConfigurableEbyte32::send(unsigned short address, unsigned char channel, const unsigned char* buffer, size_t size)
{
  unsigned char* bufferWithEverything = new unsigned char[size + 3];
  bufferWithEverything[0] = address >> 8u;
  bufferWithEverything[1] = address & 255u;
  bufferWithEverything[2] = channel;
  for (unsigned i = 0; i < size; ++i)
  {
    bufferWithEverything[i + 3] = buffer[i];
  }
  if (mode == Mode::normal)
  {
    writeInNormalMode(bufferWithEverything, size + 3);
  }
  else
  {
    Mode currentMode = mode;
    setMode(Mode::normal);
    writeInNormalMode(bufferWithEverything, size + 3);
    setMode(currentMode);
  }
  delete[] bufferWithEverything;
}

void ConfigurableEbyte32::receive(unsigned char* buffer, size_t size)
{
  if (mode == Mode::normal)
  {
    readInNormalMode(buffer, size);
  }
  else
  {
    Mode currentMode = mode;
    setMode(Mode::normal);
    readInNormalMode(buffer, size);
    setMode(currentMode);
  }
}

void ConfigurableEbyte32::resetInSleepMode()
{
  static unsigned char command[3] = {0xC4, 0xC4, 0xC4};
  serial->write(command, 3);
  waitForHighAux();
  delay(100);
}

void ConfigurableEbyte32::readConfiguration()
{
  if (mode == Mode::sleep)
  {
    readConfigurationInSleepMode();
  }
  else
  {
    Mode currentMode = mode;
    setMode(Mode::sleep);
    readConfigurationInSleepMode();
    setMode(currentMode);
  }
}

void ConfigurableEbyte32::readConfigurationInSleepMode()
{
  sendReadConfigurationCommand();
  configuration.readConfiguration(serial);
  waitForHighAux();
  isConfigurationRead = true;
}

void ConfigurableEbyte32::setConfigurationInSleepMode(bool permanent)
{
  if (permanent)
  {
    setPermanenttConfigurationInSleepMode();
  }
  else
  {
    setNonPermanenttConfigurationInSleepMode();
  }
}

void ConfigurableEbyte32::setPermanenttConfigurationInSleepMode()
{
  unsigned char command[6] = {0xC0};
  for (unsigned i = 1; i < 6; ++i)
  {
    command[i] = configuration.configurationBytes[i];
  }
  serial->write(command, 6);
  waitForHighAux();
  delay(100);
}

void ConfigurableEbyte32::setNonPermanenttConfigurationInSleepMode()
{
  unsigned char command[6] = {0xC2};
  for (unsigned i = 1; i < 6; ++i)
  {
    command[i] = configuration.configurationBytes[i];
  }
  serial->write(command, 6);
  waitForHighAux();
  delay(100);
}

String ConfigurableEbyte32::parseConfigurationToString() const
{
  return configuration.toString();
}

void ConfigurableEbyte32::sendReadConfigurationCommand()
{
  static unsigned char command[3] = {0xC1, 0xC1, 0xC1};
  serial->write(command, 3);
  waitForHighAux();
  delay(100);
}

void ConfigurableEbyte32::waitForLowAux() const
{
  while (digitalRead(aux_pin));
}

void ConfigurableEbyte32::waitForHighAux() const
{
  while (!digitalRead(aux_pin));
}

void ConfigurableEbyte32::writeInNormalMode(const unsigned char* buffer, size_t size)
{
  serial->write(buffer, size);
}

void ConfigurableEbyte32::readInNormalMode(unsigned char* buffer, size_t size)
{
  serial->readBytes(buffer, size);
}

String bytesToString(const unsigned char* bytes, unsigned length)
{
  char* buffer = new char[2 * length + 1]; 
  for (unsigned i = 0; i < length; ++i)
  {
    sprintf(buffer + 2 * i, "%02x", bytes[i]);
  }
  buffer[2 * length] = 0;
  String out = buffer;
  delete[] buffer;
  return out;
}

String addressToString(unsigned short address)
{
  char buffer[9] = {};
  sprintf(buffer, "%08x", address);
  return buffer;
}

String uartParityBitToString(ConfigurableEbyte32::Configuration::UARTParityBit uartParityBit)
{
  switch (uartParityBit)
  {
    case ConfigurableEbyte32::Configuration::UARTParityBit::v8N1:
      return "8N1";
    case ConfigurableEbyte32::Configuration::UARTParityBit::v8O1:
      return "8O1";
    case ConfigurableEbyte32::Configuration::UARTParityBit::v8E1:
      return "8E1";
  }
}

String ttlUARTBaudRateToString(ConfigurableEbyte32::Configuration::TTLUARTBaudRate ttlUARTBaudRate)
{
  switch (ttlUARTBaudRate)
  {
    case ConfigurableEbyte32::Configuration::TTLUARTBaudRate::v1200:
      return "1200";
    case ConfigurableEbyte32::Configuration::TTLUARTBaudRate::v2400:
      return "2400";
    case ConfigurableEbyte32::Configuration::TTLUARTBaudRate::v4800:
      return "4800";
    case ConfigurableEbyte32::Configuration::TTLUARTBaudRate::v9600:
      return "9600";
    case ConfigurableEbyte32::Configuration::TTLUARTBaudRate::v19200:
      return "19200:";
    case ConfigurableEbyte32::Configuration::TTLUARTBaudRate::v38400:
      return "38400";
    case ConfigurableEbyte32::Configuration::TTLUARTBaudRate::v57600:
      return "57600";
    case ConfigurableEbyte32::Configuration::TTLUARTBaudRate::v115200:
      return "115200";
  }
}

String airDataRateToString(ConfigurableEbyte32::Configuration::AirDataRate airDataRate)
{
  switch (airDataRate)
  {
    case ConfigurableEbyte32::Configuration::AirDataRate::v0_3k:
      return "0.3k";
    case ConfigurableEbyte32::Configuration::AirDataRate::v1_2k:
      return "1.2k";
    case ConfigurableEbyte32::Configuration::AirDataRate::v2_4k:
      return "2.4k";
    case ConfigurableEbyte32::Configuration::AirDataRate::v4_8k:
      return "4.8k";
    case ConfigurableEbyte32::Configuration::AirDataRate::v9_6k:
      return "9.6k";
    case ConfigurableEbyte32::Configuration::AirDataRate::v19_2k:
      return "19.2k";
  }
}

String channelToString(unsigned char channel)
{
  char buffer[9] = {};
  sprintf(buffer, "%08x", channel);
  return String(buffer) + " (" + String(862 + channel) + "Mhz)";
}

String transmissionModeToString(ConfigurableEbyte32::Configuration::TransmissionMode transmissionMode)
{
  switch (transmissionMode)
  {
    case ConfigurableEbyte32::Configuration::TransmissionMode::transparent:
      return "Transparent";
    case ConfigurableEbyte32::Configuration::TransmissionMode::fixed:
      return "Fixed";
  }
}

String ioDriveModeToString(ConfigurableEbyte32::Configuration::IODriveMode ioDriveMode)
{
  switch (ioDriveMode)
  {
    case ConfigurableEbyte32::Configuration::IODriveMode::open_collector:
      return "TXD AUX open-collector outputs, RXD open-collector inputs";
    case ConfigurableEbyte32::Configuration::IODriveMode::push_pull:
      return "TXD and AUX push-pull outputs, RXD pull-up inputs";
  }
}

String wirelessWakeUpTimeToString(ConfigurableEbyte32::Configuration::WirelessWakeUpTime wirelessWakeUpTime)
{
  switch (wirelessWakeUpTime)
  {
    case ConfigurableEbyte32::Configuration::WirelessWakeUpTime::v250:
      return "250ms";
    case ConfigurableEbyte32::Configuration::WirelessWakeUpTime::v500:
      return "500ns";
    case ConfigurableEbyte32::Configuration::WirelessWakeUpTime::v750:
      return "750ms";
    case ConfigurableEbyte32::Configuration::WirelessWakeUpTime::v1000:
      return "1000ms";
    case ConfigurableEbyte32::Configuration::WirelessWakeUpTime::v1250:
      return "1250ms";
    case ConfigurableEbyte32::Configuration::WirelessWakeUpTime::v1500:
      return "1500ms";
    case ConfigurableEbyte32::Configuration::WirelessWakeUpTime::v1750:
      return "1750ms";
    case ConfigurableEbyte32::Configuration::WirelessWakeUpTime::v2000:
      return "2000ms";
  }
}

String fecToString(ConfigurableEbyte32::Configuration::FEC fec)
{
  switch (fec)
  {
    case ConfigurableEbyte32::Configuration::FEC::off:
      return "off";
    case ConfigurableEbyte32::Configuration::FEC::on:
      return "on";
  }
}

String transmissionPowerToString(ConfigurableEbyte32::Configuration::TransmissionPower transmissionPower)
{
  switch (transmissionPower)
  {
    case ConfigurableEbyte32::Configuration::TransmissionPower::v30dBm:
      return "30dBm";
    case ConfigurableEbyte32::Configuration::TransmissionPower::v27dBm:
      return "27dBm";
    case ConfigurableEbyte32::Configuration::TransmissionPower::v24dBm:
      return "24dBm";
    case ConfigurableEbyte32::Configuration::TransmissionPower::v21dBm:
      return "21dBm";
  }
}