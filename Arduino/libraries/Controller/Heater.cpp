#include "Heater.h"

Heater::Heater(int* click_positions, int min_click, int max_click, int pin) {
  // this->servo = Servo();
  // this->click_positions = click_positions;
  // this->min_click = min_click;
  // this->max_click = max_click;
  // this->pin = pin;
  // this->last_position = -1;
}

// void Heater::set(float position) {
//   float middle_last_position = last_position + .5;
//   if (last_position == -1 ||
//       position > middle_last_position + DIAL_THRESHOLD ||
//       position < middle_last_position - DIAL_THRESHOLD) {
//     setUnfiltered(position);
//   }
// }

// void Heater::setUnfiltered(float position) {
//   if (last_position != (int)position) {
//     forceSet(position);
//   }
// }

// void Heater::forceSet(float position) {
//   last_position = (int)position;
//   int angle = click_positions[last_position];
//   setServo(angle);
// }

// void Heater::setServo(int angle) {
//   servo.attach(pin);
//   servo.write(angle);
//   delay(1000);
//   servo.detach();
// }
