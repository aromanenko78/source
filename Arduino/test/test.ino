#include <Servo.h>
#include <Wire.h>
#include "DialServo.h"

int heat_clicks[] = { 0, 1, 2 };

DialServo heat_dial_servo(heat_clicks, 1, 3, 5);

// test.ino

void setup() {

}

void loop() {

}

