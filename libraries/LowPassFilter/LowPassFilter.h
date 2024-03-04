#ifndef LOW_PASS_FILTER
#define LOW_PASS_FILTER

class LowPassFilter
{
public:
  LowPassFilter(float coff1, float coff2);
  void init(float x);
  float step(float x);
private:
  float coff1;
  float coff2;
  float x;
  float y;
};

#endif
