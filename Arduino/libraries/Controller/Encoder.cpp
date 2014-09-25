#include "Encoder.h"

volatile unsigned int b;
volatile unsigned int p2;
volatile int counter;
volatile unsigned long ignore_button_press_until;

volatile bool button_pressed;
volatile int dialValue;
int minValue;
int maxValue;

void buttonChanged() {
  unsigned int bn = digitalRead(BUTTON_PIN);
  unsigned long button_press_time = millis();
  if (b != bn) {
    if (ignore_button_press_until < button_press_time) {
      if (bn == 0) {
        // TODO: handler B
        //Serial.println("button");
        button_pressed = true;
      }
    }
    b = bn;
  }
  ignore_button_press_until = button_press_time + 50;
}

void encoderChanged() {
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
    //Serial.println("encoder r");
    dialValue++;
    if (dialValue > maxValue) {
      dialValue = maxValue;
    }

  } else if (counter < -1) {
    counter = 0;
    // TODO: handler L
    //Serial.println("encoder l");
    dialValue--;
    if (dialValue < minValue) {
      dialValue = minValue;
    }
  }
}

void Encoder::init(int initialValue, int minValue, int maxValue) {
  b = 1;
  p2 = 1;
  dialValue = initialValue;

  pinMode(BUTTON_PIN, INPUT);
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  digitalWrite(BUTTON_PIN, HIGH);
  digitalWrite(ENCODER_PIN_A, HIGH);
  digitalWrite(ENCODER_PIN_B, HIGH);
  attachInterrupt(BUTTON_INTERRUPT, buttonChanged, CHANGE); 
  attachInterrupt(ENCODER_INTERRUPT, encoderChanged, CHANGE); 
}

bool Encoder::getButtonPressed() {
  if (button_pressed) {
    button_pressed = false;
    return true;
  }
  return false;
}

int Encoder::getDialValue() {
  return dialValue;
}
