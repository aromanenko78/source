#include "MedianFilter.h"

MedianFilter::MedianFilter(int window) {
  this->index = -1;
  this->window = window;
  this->values = new float[window];
  this->sorted_values = new float[window];
  this->median_index = (window / 2) + 1;
}

float MedianFilter::apply(float value, float target) {
  error = value - target;

  if (index == -1) {
    for (int i = 0; i < window; i++) {
      values[i] = value;
      sorted_values[i] = value;
    }
    return value;
  }

  index++;
  if (index > window) {
    index = 0;
  }

  // update values
  float outgoing = values[index];
  values[index] = value;

  // find in sorted values
  int sorted_index = 0;
  while (sorted_values[sorted_index] > outgoing) {
    sorted_index++;
  }

  // update sorted values
  sorted_values[sorted_index] = value;

  // bubble up
  for (int i = sorted_index + 1; i < window; i++) {
    if (!CompareAndSwap(i - 1, i)) {
      break;
    }
  }

  // sink down
  for (int i = sorted_index - 1; i > 0; i--) {
    if (!CompareAndSwap(i, i + 1)) {
      break;
    }
  }

  return sorted_values[median_index];
}

bool MedianFilter::CompareAndSwap(int index1, int index2) {
    if (sorted_values[index1] < sorted_values[index2]) {
      float temp = sorted_values[index1];
      sorted_values[index1] = sorted_values[index2];
      sorted_values[index2] = temp;
      return true;
    }
    return false;
}
