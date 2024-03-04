#include "LowPassFilter.h"

LowPassFilter::LowPassFilter(float coff1, float coff2)
  : coff1{coff1}, coff2{coff2}
{}

void LowPassFilter::init(float x)
{
  this->x = this->y = x;
}

float LowPassFilter::step(float x)
{
  y = (this->x + x + coff2 * y) / coff1;
  this->x = x;
  return y;
}