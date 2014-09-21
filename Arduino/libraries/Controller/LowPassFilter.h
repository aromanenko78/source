#ifndef LowPassFilter_H
#define LowPassFilter_H
class LowPassFilter {
 private:
  float lpf;
  float last;
  float alpha;
  float error;
  bool initialized;

 public:
  LowPassFilter(float alpha);
  float apply(float value, float target);
  float getError() { return error; }
  float getLPF() { return lpf; }
  float getUnfiltered() { return last; }
  float getDelta() { return last - lpf; }
};
#endif
