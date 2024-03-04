#ifndef _PID
#define _PID

class PID
{
public:
  PID(float dt, float P, float I, float D);
  float step(float error);
  void reset();
  void change(float P, float I, float D);
private:
  const float dt;
  float P;
  float I;
  float D;
  float previousError = 0.0f;
  float integralSum = 0.0f;
};

#endif
