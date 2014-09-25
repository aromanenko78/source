#ifndef Encoder_H
#define Encoder_H

#include "Arduino.h"

#define BUTTON_PIN 2
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 13

#define BUTTON_INTERRUPT 0
#define ENCODER_INTERRUPT 1

class Encoder {
  public:
    void init(int initialValue, int minValue, int maxValue);
    bool getButtonPressed();
    int getDialValue();
};
#endif
