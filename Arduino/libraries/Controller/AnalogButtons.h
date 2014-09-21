#ifndef AnalogButtons_H
#define AnalogButtons_H

#include "Arduino.h"

class AnalogButtons {
 private:
  static const int VALUES_LEN;
  static const int VALUES[];
  int analogPin;

 public:
  AnalogButtons(int analogPin);
  int get();
};
#endif
