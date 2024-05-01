#include <HX711_ADC.h>

const unsigned long dt = 3000;
unsigned long testStartTime = 0;
unsigned long loopTime = 0;

float motorSpeed = 0.0f;
float maxMotorSpeed = 1.0f; 

void setMotorTimers()
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

void setMotorsPWM()
{
  TIMER2_BASE->CCR1 = 1000 + int16_t(motorSpeed * 1000);
  TIMER2_BASE->CCR2 = 1000 + int16_t(motorSpeed * 1000);
  TIMER2_BASE->CCR3 = 1000 + int16_t(motorSpeed * 1000);
  TIMER2_BASE->CCR4 = 1000 + int16_t(motorSpeed * 1000);
  TIMER2_BASE->CNT = 5000;
}

//pins:
const int HX711_dout = PB12; //mcu > HX711 dout pin
const int HX711_sck = PB13; //mcu > HX711 sck pin
HX711_ADC LoadCell(HX711_dout, HX711_sck);

float data[3] = {0.0f, 0.0f, 0.0f};

void setup() 
{
  Serial1.begin(1000000);
  setMotorTimers();
  motorSpeed = 0.0f;
  setMotorsPWM();

  LoadCell.begin();
  LoadCell.start(5000, true);
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) 
  {
    while (1);
  }
  else 
  {
    LoadCell.setCalFactor(1.0);
  }
  while (!LoadCell.update());
  LoadCell.setCalFactor(500.0f);

  motorSpeed = 0.0f;
  setMotorsPWM();
  delay(2000);
  testStartTime = millis();
}

void motorSpeedUpdate()
{
  unsigned long ind = (loopTime - testStartTime) / dt;
  if (ind < 5)
  {
    motorSpeed = 0.0f;
  }
  else
  {
    motorSpeed = 0.01f * (ind - 5);
    motorSpeed = max(0.0f, min(motorSpeed, 1.0f));
    if (motorSpeed > maxMotorSpeed)
    {
      motorSpeed = 0.0f;
    }
  }
}

void loop() 
{
  loopTime = millis();
  motorSpeedUpdate();
  setMotorsPWM();
  if (LoadCell.update()) 
  {
    data[0] = (loopTime - testStartTime) / 1000.0f;
    data[1] = motorSpeed;
    data[2] = LoadCell.getData();
    Serial1.write((const uint8_t*)data, sizeof(data));
  }
  delay(3);
}
