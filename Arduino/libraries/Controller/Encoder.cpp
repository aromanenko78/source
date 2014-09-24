#include "Encoder.h"

Encoder* instance;

void buttonChangedStatic() {
  if (instance) {
    instance->buttonChanged();
  }
}

void encoderChangedStatic() {
  if (instance) {
    instance->encoderChanged();
  }
}

void Encoder::init() {
  b = 1;
  p2 = 1;
  instance = this;

  pinMode(BUTTON_PIN, INPUT);
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  digitalWrite(ENCODER_PIN_A, HIGH);
  digitalWrite(ENCODER_PIN_B, HIGH);
  attachInterrupt(BUTTON_INTERRUPT, buttonChangedStatic, CHANGE); 
  attachInterrupt(ENCODER_INTERRUPT, encoderChangedStatic, CHANGE); 
}

void Encoder::buttonChanged() {
  unsigned int bn = digitalRead(BUTTON_PIN);
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

void Encoder::encoderChanged() {
  unsigned int p2n = digitalRead(ENCODER_PIN_A);

  if (p2 != p2n) {
    p2 = p2n;
    unsigned int p3 = digitalRead(ENCODER_PIN_B);

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

