#ifndef COLORS
#define COLORS

#include "Arduino.h"

enum class Color : uint16_t
{
  black = 0x0000,
  navy = 0x000F,
  darkGreen = 0x03E0,
  darkCyan = 0x03EF,
  maroon = 0x7800,
  purple = 0x780F,
  olive = 0x7BE0,
  lightGrey = 0xC618,
  darkGrey = 0x7BEF,
  blue = 0x001F,
  green = 0x07E0,
  cyan = 0x07FF,
  red = 0xF800,
  magenta = 0xF81F,
  yellow = 0xFFE0,
  white = 0xFFFF,
  orange = 0xFD20,
  greenYellow = 0xAFE5
};

String colorToString(Color color);
Color stringToColor(const String& string);

#endif
