#include "LowPassFilter.h"

LowPassFilter::LowPassFilter(float alpha) {
  this->alpha = alpha;
  this->initialized = false;
}

float LowPassFilter::apply(float value, float target) {
  if (!initialized) {
    lpf = value;
    initialized = true;
  } else {
    lpf = lpf + (value - lpf) * alpha;
  }

  last = value;
  error = value - target;
  return lpf;
}
