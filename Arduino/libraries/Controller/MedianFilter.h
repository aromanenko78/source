#ifndef MedianFilter_H
#define MedianFilter_H
class MedianFilter {
 private:
  int index;
  int window;
  int median_index;
  float* values;
  float* sorted_values;
  float error;

  bool CompareAndSwap(int index1, int index2);

 public:
  MedianFilter(int window);
  float apply(float value, float target);
  float getLPF() { return sorted_values[median_index]; }
  float getError() { return error; }
};
#endif
