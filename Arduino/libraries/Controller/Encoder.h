#ifndef Encoder_H
#define Encoder_H

#include "Arduino.h"

#define BUTTON_PIN 2
#define ENCODER_PIN_A 3
#define ENCODER_PIN_B 13

#define BUTTON_INTERRUPT 0
#define ENCODER_INTERRUPT 1

class Encoder {
  private:
    volatile unsigned int b;
    volatile unsigned int p2;
    volatile int counter;
    volatile unsigned long ignore_button_press_until;
    //static Encoder* instance;

    //static void buttonChangedStatic();
    //static void encoderChangedStatic();

  public:
    void init();
    void buttonChanged();
    void encoderChanged();
};
#endif
