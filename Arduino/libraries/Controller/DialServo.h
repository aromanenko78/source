#ifndef DialServo_H
#define DialServo_H

#include <Servo.h>
#include "Arduino.h"

#define DIAL_THRESHOLD .7

class DialServo {
 private:
  int pin;
  int last_position;
  int* click_positions;
  int min_click;
  int max_click;
  Servo servo;
  void setServo(int angle);

 public:
  DialServo(int* click_positions, int min_click, int max_click, int pin);
  void set(float position);
  void setUnfiltered(float position);
  void forceSet(float position);
};
#endif
