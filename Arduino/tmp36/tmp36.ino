#include "LowPassFilter.h"
#include "MedianFilter.h"

#define ANALOG_TEMP_PIN 0

LowPassFilter lpf(.01);
MedianFilter mf(3);

void setup() {
  Serial.begin(9600);
  Serial.println("Ready");
}

void loop() {
  float temp3 = (getvoltage(ANALOG_TEMP_PIN) - .5) * 100;
  
  Serial.print(temp3);
  Serial.print(", ");
  Serial.print(lpf.apply(temp3));
  Serial.print(", ");
  Serial.println(mf.apply(temp3));
  delay(1 * 50);
}

float getvoltage(int pin) {
  return (analogRead(pin) * .004882814);
}

