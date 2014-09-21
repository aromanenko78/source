#include <Serial.h>

volatile unsigned int b = 1;
volatile unsigned int p2 = 1;
volatile int counter = 0;
volatile unsigned long ignore_button_press_until = 0;

void setup() {
  // Button pin 2
  // Encoder pin1 3
  // Encoder pin2 13
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(13, INPUT);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(13, HIGH);
  attachInterrupt(0, buttonChanged, CHANGE); 
  attachInterrupt(1, encoderChanged, CHANGE); 
  Serial.begin(9600);
  Serial.println("begin");
}

void loop() {
  delay(10000);
}

void buttonChanged() {
  unsigned int bn = digitalRead(2);
  unsigned long button_press_time = millis();
  if (b != bn) {
    if (ignore_button_press_until < button_press_time) {
      if (bn == 0) {
        // TODO: handler B
        Serial.println("button");
      }
    }
    b = bn;
  }
  ignore_button_press_until = button_press_time + 50;
}

void encoderChanged() {
  unsigned int p2n = digitalRead(3);

  if (p2 != p2n) {
    p2 = p2n;
    unsigned int p3 = digitalRead(13);

    if (p2 == 1) {
      counter += (p3 << 1) - 1;  // if (p3 == 0) { counter--; } else { counter++; }
    } else {
      counter += 1 - (p3 << 1);  // if (p3 == 0) { counter++; } else { counter--; }
    }
  }

  if (counter > 1) {
    counter = 0;
    // TODO: handler R
    Serial.println("encoder r");
  } else if (counter < -1) {
    counter = 0;
    // TODO: handler L
    Serial.println("encoder l");
  }
}
