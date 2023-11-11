#ifndef EBYTE_32
#include "Arduino.h"

namespace
{
bool validateBuffer(const unsigned char* buffer, int size, int N)
{
  for (int i = 0; i < N; ++i)
  {
    if (buffer[i] != 'S' + i || buffer[size - 1 - i] != 'E' + i)
    {
      return false;
    }
  }
  return true;
}
}


class Ebyte32
{
public:
  Ebyte32(HardwareSerial* serial, int aux_pin, int baudRate=115200);
  void begin();
  template<typename T, int N=0>
  void send(unsigned short address, unsigned char channel, const T& msg)
  {
    static constexpr int size = sizeof(T) + 2 * N + 3;
    unsigned char buffer[size];
    buffer[0] = address >> 8u;
    buffer[1] = address & 255u;
    buffer[2] = channel;
    for (int i = 0; i < N; ++i)
    {
      buffer[i + 3] =  'S' + i;
    }
    for (int i = 0; i < N; ++i)
    {
      buffer[size - 1 - i] =  'E' + i;
    }
    const unsigned char* data = reinterpret_cast<const unsigned char*>(&msg);
    for (int i = 0; i < sizeof(T); ++i)
    {
      buffer[N + 3 + i] = data[i];
    }

    while (serial->availableForWrite() < size);
    serial->write(buffer, size);
  }
  template<typename T, int N=0>
  bool receive(T& msg)
  {
    static constexpr int size = sizeof(T) + 2 * N;
    if (!msgShift)
    {
      unsigned char buffer[size];
      serial->readBytes(buffer, size);
      if (validateBuffer(buffer, size, N))
      {
        msg = *reinterpret_cast<const T*>(buffer + N);
        return true;
      }
      msgShift = true;
      return false;
    }
    else
    {
      if (serial->available() <= 2 * size)
      {
        return false;
      }
      unsigned char buffer[size];
      serial->readBytes(buffer, size);

      bool found = false;
      int ind = 0;
      for (int i = 1; i < size && !found; ++i)
      {
        if (buffer[i] == 'S' && buffer[i - 1] == 'E')
        {
          found = true;
          for (int k = 1; k < N; ++k)
          {
            int Sind = (i + k) % size;
            int Eind = (size + i - k - 1) % size;
            if (buffer[Sind] != 'S' + k || buffer[Eind] != 'E' + k)
            {
              found = false;
              break;
            }
          }
          if (found)
          {
            ind = i;
            break;
          }
        }
      }
      if (found)
      {
        for (int i = 0; i < size - ind; ++i)
        {
          buffer[i] = buffer[i + ind];
        }
        serial->readBytes(buffer + size - ind , ind);
        if (validateBuffer(buffer, size, N))
        {
          msg = *reinterpret_cast<const T*>(buffer + N);
          msgShift = false;
          return true;
        }
      }
      return false;
    }
  }
  template<typename T, int N=0>
  bool avalibleToReceive()
  {
    return serial->available() >= sizeof(T) + 2 * N;
  }
private:
  HardwareSerial* serial;
  const int aux_pin;
  int baudRate;
  bool msgShift = false;
};

#endif
