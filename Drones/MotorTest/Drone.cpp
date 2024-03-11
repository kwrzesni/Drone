#include  "Drone.h"
#include <Wire.h>

void Drone::begin()
{
  // begin motors
  setMotorTimers();
  motor0Speed = motor1Speed = motor2Speed = motor3Speed = 1.0f;
  setMotorsPWM();
  testStartTime = micros();
}

void Drone::step()
{
  stepStartTime = micros();
  setMotorsSpeed();
  waitTillEndOfStep();
}

void Drone::setMotorTimers()
{
  TIMER2_BASE->CR1 = TIMER_CR1_CEN | TIMER_CR1_ARPE;
  TIMER2_BASE->CR2 = 0;
  TIMER2_BASE->SMCR = 0;
  TIMER2_BASE->DIER = 0;
  TIMER2_BASE->EGR = 0;
  TIMER2_BASE->CCMR1 = (0b110 << 4) | TIMER_CCMR1_OC1PE |(0b110 << 12) | TIMER_CCMR1_OC2PE;
  TIMER2_BASE->CCMR2 = (0b110 << 4) | TIMER_CCMR2_OC3PE |(0b110 << 12) | TIMER_CCMR2_OC4PE;
  TIMER2_BASE->CCER = TIMER_CCER_CC1E | TIMER_CCER_CC2E | TIMER_CCER_CC3E | TIMER_CCER_CC4E;
  TIMER2_BASE->PSC = 71;
  TIMER2_BASE->DCR = 0;

  TIMER2_BASE->CCR1 = 1000;
  TIMER2_BASE->CCR2 = 1000;
  TIMER2_BASE->CCR3 = 1000;
  TIMER2_BASE->CCR4 = 1000;
  pinMode(PA0, PWM);
  pinMode(PA1, PWM);
  pinMode(PA2, PWM);
  pinMode(PA3, PWM);
  TIMER2_BASE->ARR = 5000;
}

void Drone::setMotorsSpeed()
{
  if (micros() - testStartTime < 5000000)
  {
    motor0Speed = motor1Speed = motor2Speed = motor3Speed = 1.0;
  }
  else if (micros() - testStartTime < 10000000)
  {
    motor0Speed = motor1Speed = motor2Speed = motor3Speed = 0.0;
  }
  else if (micros() - testStartTime < 20000000)
  {
    motor0Speed = motor1Speed = motor2Speed = motor3Speed = (micros() - testStartTime - 10000000) / float(10000000);
  }
  else
  {
    motor0Speed = motor1Speed = motor2Speed = motor3Speed = 0;
  }
  setMotorsPWM();
}

void Drone::setMotorsPWM()
{
  TIMER2_BASE->CCR1 = 1000 + int16_t(motor1Speed * 1000);
  TIMER2_BASE->CCR2 = 1000 + int16_t(motor2Speed * 1000);
  TIMER2_BASE->CCR3 = 1000 + int16_t(motor0Speed * 1000);
  TIMER2_BASE->CCR4 = 1000 + int16_t(motor3Speed * 1000);
  TIMER2_BASE->CNT = 5000;
}

void Drone::waitTillEndOfStep()
{
  while (micros() - stepStartTime < STEP_TIME);
}