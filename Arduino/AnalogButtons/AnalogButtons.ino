/*
  Analog Input
  100Kohm pulldown
  10Kohm x 3 per button + 1 button to +5 directly
  
  Values: 
  1014, 923, 850, 783, 20 (none pressed)
 */
#include <Servo.h>
#include "AnalogButtons.h"

int analogPin = A0;    // select the input pin for the potentiometer
AnalogButtons buttons(analogPin);

void setup() {
  Serial.begin(9600);
}

void loop() {
  //int sensorValue = analogRead(sensorPin);
  int button = buttons.get();
  Serial.println(button);
  delay(100);
}
