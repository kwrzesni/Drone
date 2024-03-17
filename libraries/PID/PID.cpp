#include "PID.h"

float clamp(float value, float min, float max)
{
  if (value < min)
  {
    return min;
  }
  if (value > max)
  {
    return max;
  }
  return value;
}


PID::PID(float P, float I, float D, float limit)
  : P{P}, I{I}, D{D}, limit{limit}
{}

float PID::step(float error)
{
  integralSum = clamp(integralSum + I * error, -limit, limit);
  float PIDOutput = clamp(P * error + integralSum + D * (error - previousError), -limit, limit);
  previousError = error;
  return PIDOutput;
}

void PID::reset()
{
  previousError = 0.0f;
  integralSum = 0.0f;
}

void PID::change(float P, float I, float D)
{
  if (this->P != P || this->I != I || this->D != D)
  {
    this->P = P;
    this->I = I;
    this->D != D;
    previousError = 0.0f;
    integralSum = 0.0f;
  }
}

void PID::changeP(float P)
{
  this->P = P;
  previousError = 0.0f;
  integralSum = 0.0f;
}

void PID::changeI(float I)
{
  this->I = I;
  previousError = 0.0f;
  integralSum = 0.0f;
}

void PID::changeD(float D)
{
  this->D = D;
  previousError = 0.0f;
  integralSum = 0.0f;
}