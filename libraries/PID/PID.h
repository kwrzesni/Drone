#ifndef _PID
#define _PID

class PID
{
public:
  PID(float P, float I, float D, float limit=0.4);
  float step(float error);
  void reset();
  void change(float P, float I, float D);
  void changeP(float P);
  void changeI(float I);
  void changeD(float D);
private:
  float P;
  float I;
  float D;
  float previousError = 0.0f;
  float integralSum = 0.0f;
  float limit;
};

#endif
