#include "AnalogButtons.h"
#include "limits.h"

const int AnalogButtons::VALUES_LEN = 5;
const int AnalogButtons::VALUES[VALUES_LEN] = {20, 783, 850, 923, 1014};

AnalogButtons::AnalogButtons(int analogPin) {
  this->analogPin = analogPin;
}

int AnalogButtons::get() {
  int value = analogRead(analogPin);
  int delta = INT_MAX;
  int result = 0;

  for (int i = 0; i < VALUES_LEN; i ++) {
    int currentDelta = abs(VALUES[i] - value);
    if (currentDelta >= delta) {
      break;
    }
    delta = currentDelta;
    result = i;
  }

  return result;
}
