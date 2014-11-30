#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <Limits.h>
#include <OneWire.h>
#include <Servo.h>
#include <Time.h>
#include <Wire.h>
#include "DialServo.h"
#include "Encoder.h"
#include "Heater.h"
#include "LowPassFilter.h"
#include "MedianFilter.h"

#define TARGET 25
#define MIN_SETTING 10
#define MAX_SETTING 40

Heater heater;
Encoder encoder;
LiquidCrystal lcd(11, 12, 4, 5, 6, 7);
bool on = true;
unsigned long nextSlowLoop;
unsigned long showMessageUntil;

void setup() {
  // contrast
  analogWrite(2, 320);

  lcd.begin(16, 2);
  lcd.print("Starting...");
  Serial.begin(9600);
  Serial.println("Ready");

  heater.init(TARGET, MIN_SETTING, MAX_SETTING);
  encoder.init(TARGET, MIN_SETTING - 1, MAX_SETTING + 1);
}

void loop() {
  if (encoder.getButtonPressed()) {
    if (on) {
      on = false;
      showMessage("OFF");
      heater.flowOff();
      update(true);
    } else {
      on = true;
      showMessage("ON");
      heater.flowOn();
      update(true);
    }
  } else if (encoder.getDialChanged()) {
    int target = encoder.getDialValue();
    if (target > MAX_SETTING) {
      showMessage("SET  MAX");
    } else if (target < MIN_SETTING) {
      showMessage("SET  MIN");
    } else {
      showTempMessage("SET  ", target);
    }
    heater.setTempTarget(target);
    nextSlowLoop = millis() + 3000;
  } else if (millis() > nextSlowLoop) {
    update(false);
  }
}

void showMessage(char* message) {
  lcd.setCursor(0, 0);
  lcd.print(message);
  lcd.print("              ");
  showMessageUntil = millis() + 3000;
}

void showTempMessage(char* message, int value) {
  lcd.setCursor(0, 0);
  lcd.print(message);
  lcd.print(value);
  lcd.print("              ");
  showMessageUntil = millis() + 3000;
}

void update(bool force) {
  heater.updateDials(force);
  updateLcd();
  heater.trace();
  nextSlowLoop = millis() + 1000;
}

void updateLcd() {
  if (millis() > showMessageUntil) {
    lcd.setCursor(0, 0);
    lcd.print("temp ");
    lcd.print(heater.getIntTemp());
    lcd.print("  vent ");
    lcd.print(heater.getVentTemp());
    lcd.print("    ");
  }

  lcd.setCursor(0, 1);
  lcd.print("heat ");
  lcd.print(heater.getHeatDialPosition());
  lcd.print("  flow ");
  lcd.print(heater.getFlowDialPosition());
  lcd.print("    ");
}
