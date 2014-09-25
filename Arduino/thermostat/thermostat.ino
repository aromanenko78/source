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

Heater heater;
Encoder encoder;
LiquidCrystal lcd(11, 12, 4, 5, 6, 7);

void setup() {
  // contrast
  analogWrite(2, 320);

  lcd.begin(16, 2);
  lcd.print("Starting...");
  Serial.begin(9600);
  Serial.println("Ready");

  heater.init();
  encoder.init(25, 10, 40);
}

void loop() {
  heater.updateDials(false);
  heater.trace();
  updateLcd();

  delay(1 * 1000); // was 5 sec
}

void updateLcd() {
  lcd.setCursor(0, 0);
  lcd.print("temp ");
  lcd.print(heater.getIntTemp());
  lcd.print("  vent ");
  lcd.print(heater.getVentTemp());
  lcd.print("    ");

  lcd.setCursor(0, 1);
  lcd.print("heat ");
  lcd.print(heater.getHeatDialPosition());
  lcd.print("  flow ");
  lcd.print(heater.getFlowDialPosition());
  lcd.print("    ");
}
