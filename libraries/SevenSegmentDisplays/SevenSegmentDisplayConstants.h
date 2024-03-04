#ifndef SEVEN_SEGMENT_DISPLAY_CONSTANTS
#define SEVEN_SEGMENT_DISPLAY_CONSTANTS
#include "Arduino.h"
#include <map>
#include <array>

extern std::map<char, std::array<uint8, 8>> SYMBOL_TO_SEGMENTS;
extern std::map<char, uint8> SYMBOL_TO_SEGMENTS_REGISTER;
#endif
