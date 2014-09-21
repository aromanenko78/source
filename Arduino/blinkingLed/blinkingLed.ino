
int ledPin = 9;
int maxBrightness = 100;
int delayMs = 5;

void setup() {
  pinMode(ledPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < maxBrightness; i++) {
    analogWrite(ledPin, i);
    delay(delayMs);
  }
  for (int i = maxBrightness; i >= 0; i--) {
    analogWrite(ledPin, i);
    delay(delayMs);
  }
  delay(1000);
  //digitalWrite(ledPin, HIGH);
  //delay(1000);
  //digitalWrite(ledPin, LOW);
  //delay(1000);
}
