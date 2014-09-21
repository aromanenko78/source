// keys.ino
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include "ExtenderKeys.h"

ExtenderKeys mykeys(0x20);

void onInterrupt() {
  Serial.println("onInterrupt");
  mykeys.processInterrupt();
}

void keyHandlerA() {
  Serial.println("A pressed");
}

void setup() {
  Serial.begin(9600);
  Serial.println("ready");
  attachInterrupt(0, onInterrupt, FALLING);
  mykeys.begin();
  //mykeys.attachKeyHandler(0, keyHandlerA);
}

void loop() {
  delay(2000);
  Serial.println("loop");
  mykeys.processKeys();
}
