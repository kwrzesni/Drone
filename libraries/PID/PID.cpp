#include "PID.h"

PID::PID(float dt, float P, float I, float D)
  : dt{dt}, P{P}, I{I}, D{D}
{}

float PID::step(float error)
{
  float Pterm = P * error;
  float Iterm = integralSum + I * (error + previousError) * dt / 2;
  if (Iterm > 0.4)
  {
    Iterm = 0.4;
  } 
  else if (Iterm < - 0.4) 
  {
    Iterm = -0.4;
  }
  float Dterm= D * (error - previousError) / dt;
  float PIDOutput = Pterm+Iterm+Dterm;
  if (PIDOutput > 0.4)
  {
    PIDOutput = 0.4;
  }
  else if (PIDOutput < -0.4)
  {
    PIDOutput = -0.4;
  } 
  previousError = error;
  integralSum = Iterm;
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