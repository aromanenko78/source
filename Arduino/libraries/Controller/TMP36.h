#ifndef TMP36_H
#define TMP36_H

#include "Arduino.h"

class TMP36 {
 private:
  int pin;
  float getVoltage();

 public:
  TMP36(int pin);
  float getTemp();
};
#endif
