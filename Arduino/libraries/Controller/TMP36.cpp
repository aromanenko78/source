#include "TMP36.h"

TMP36::TMP36(int pin) {
  this->pin = pin;
}

float TMP36::getTemp() {
  return (getVoltage() - .5) * 100;
}

float TMP36::getVoltage() {
  return (analogRead(pin) * .004882814);
}
