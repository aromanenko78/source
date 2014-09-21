// analogRead.ino
#include <Serial.h>

void setup() {
  Serial.begin(9600);
  Serial.println("begin");
}

void loop() {
  Serial.println(analogRead(2));
  delay(2000);
}

