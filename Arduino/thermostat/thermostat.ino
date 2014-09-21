// Bread board: LCD 12, 11, 7, 6, 5, 4
// Board: LCD 12, 11, 4, 5, 6, 7
 
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <Limits.h>
#include <OneWire.h>
#include <Servo.h>
#include <Time.h>
#include <Wire.h>
#include "AnalogButtons.h"
#include "DialServo.h"
#include "LowPassFilter.h"
#include "MedianFilter.h"

// Pin layout
#define BUTTONS_ANALOG_PIN 0
#define HEAT_SERVO_PIN 9
#define FLOW_SERVO_PIN 10
#define RESET_BUTTON_PIN 6
#define ONE_WIRE_BUS 8
#define TEMPERATURE_PRECISION 12

// Buttons
#define ON_OFF_BUTTON 1
#define TEMP_DOWN_BUTTON 2
#define TEMP_UP_BUTTON 3
#define RESET_BUTTON 4

// Dial settings
#define HEAT_CLICKS { 0, 7, 17, 27, 39, 53, 65, 77, 90, 103, 115, 128, 139, 150 }
#define FLOW_CLICKS { 10, 37, 48, 63, 77, 89, 101, 114, 131, 145 }
#define MIN_HEAT 0
#define MAX_HEAT 13
#define MIN_FLOW 0
#define MAX_FLOW 9
#define MIN_HEAT_DIAL MIN_HEAT - MIN_FLOW - 2
#define MAX_HEAT_DIAL MAX_HEAT + MAX_FLOW + 2

// buttons: temp up, temp down, flow up, flow down, auto, off
// 3rd servo & window, high, low

// TODO: buttons and menus

float temp_target = 24.5;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress ventThermometer, insideThermometer;
int heat_clicks[] = HEAT_CLICKS;
int flow_clicks[] = FLOW_CLICKS;
DialServo heat_dial_servo(heat_clicks, MIN_HEAT + .5, MAX_HEAT, HEAT_SERVO_PIN);
DialServo flow_dial_servo(flow_clicks, MIN_FLOW + 1.5, MAX_FLOW, FLOW_SERVO_PIN);
LowPassFilter vent_temp_lpf(.1);
//MedianFilter int_temp_lpf(3);
LowPassFilter int_temp_lpf(.05); // #13 .02 -> .03; #14 .03 -> .05
LowPassFilter heat_dial_lpf(.08);
LowPassFilter flow_dial_lpf(.08); // #14 .3 -> .05, #15 .05 -> .08
LiquidCrystal lcd(11, 12, 4, 5, 6, 7);
AnalogButtons buttons(BUTTONS_ANALOG_PIN);
boolean resetWasPressed = false;
int loop_counter = 0;
int loop_size = 1000;
int adjustment_counter = 0;

void setup() {
  // contrast
  analogWrite(2, 320);

  lcd.begin(16, 2);
  lcd.print("Starting...");
  Serial.begin(9600);
  Serial.println("Ready");

  flow_dial_servo.forceSet(0);

  delay(3 * 1000);

  pinMode(RESET_BUTTON_PIN, INPUT);

  sensors.begin();
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 1"); 
  if (!sensors.getAddress(ventThermometer, 1)) Serial.println("Unable to find address for Device 0"); 
  sensors.setResolution(ventThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.requestTemperatures();

  // select initial dial position
  float initial_temp = sensors.getTempC(insideThermometer);
  float initial_heat_dial = 5;
  if (initial_temp < temp_target - 5) {
    initial_heat_dial = MAX_HEAT;
  } else if (initial_temp > temp_target + 5) {
    initial_heat_dial = MIN_HEAT;
  }

  heat_dial_lpf.apply(initial_heat_dial, initial_heat_dial);
  heat_dial_servo.forceSet(initial_heat_dial);
}

void loop() {
  int button = buttons.get();
  if (button > 0) {
    if (button == RESET_BUTTON) {
      resetWasPressed = true;
      heat_dial_servo.forceSet(MIN_HEAT);
      flow_dial_servo.forceSet(MIN_FLOW);
      Serial.println("Reset");
      return;
    }
  } else if (resetWasPressed) {
    resetWasPressed = false;
    heat_dial_servo.forceSet(MAX_HEAT);
    flow_dial_servo.forceSet(MAX_FLOW);
    loop_counter = INT_MAX - 1; // set dials back on next iteration
    Serial.println("Post Reset");
    return;
  }

  Serial.println("Normal flow");
  update();
}

void update() {
  sensors.requestTemperatures();
  int_temp_lpf.apply(sensors.getTempC(insideThermometer), temp_target);
  vent_temp_lpf.apply(sensors.getTempC(ventThermometer), temp_target);

  // .4 coefficient is very stable
  float heat_dial_limited = heat_dial_lpf.getLPF() - .4 * (int_temp_lpf.getError() + vent_temp_lpf.getError() / 2);
  if (heat_dial_limited > MAX_HEAT_DIAL) {
    heat_dial_limited = MAX_HEAT_DIAL;
  } else if (heat_dial_limited < MIN_HEAT_DIAL) {
    heat_dial_limited = MIN_HEAT_DIAL;
  }
  heat_dial_lpf.apply(heat_dial_limited, heat_dial_lpf.getLPF());
  float heat_dial = heat_dial_lpf.getUnfiltered();
  
  float flow_dial = 3;

  if (abs(int_temp_lpf.getError()) > 1) {
    flow_dial += abs(int_temp_lpf.getError()) * .5 - 1;
  }

  // limit heat dial, increase flow if heat dial is not sufficient
  if (heat_dial < MIN_HEAT) {
    flow_dial += MIN_HEAT - heat_dial;
    heat_dial = MIN_HEAT;
  } else if (heat_dial > MAX_HEAT) {
    flow_dial += heat_dial - MAX_HEAT;
    heat_dial = MAX_HEAT;
  }
  
  // engine initial warm up correction
  if (vent_temp_lpf.getLPF() < temp_target + 10 &&
      heat_dial == MAX_HEAT) {
    float warmup_max_flow = vent_temp_lpf.getLPF() - temp_target;
    if (vent_temp_lpf.getLPF() > int_temp_lpf.getLPF() && warmup_max_flow < 1) {
      warmup_max_flow = 1;
    }

    if (flow_dial > warmup_max_flow) {
      flow_dial = warmup_max_flow;
    }
  }

  // limit flow dial
  if (flow_dial < MIN_FLOW) {
    flow_dial = MIN_FLOW;
  } else if (flow_dial > MAX_FLOW) {
    flow_dial = MAX_FLOW;
  }
  
  // update flow
  flow_dial_lpf.apply(flow_dial + .5, flow_dial_lpf.getLPF());
  flow_dial = (int)flow_dial_lpf.getLPF();

  // stability score, the lower the more stable
  float stability_score = abs(vent_temp_lpf.getError()) + 2 * abs(vent_temp_lpf.getDelta()); // + abs(int_temp_lpf.getError());
  int adjustment_interval;
  adjustment_counter++;
  
  if (stability_score < 1) {
    adjustment_interval = 30;
  } else if (stability_score < 2) {
    adjustment_interval = 15;
  } else if (stability_score < 3) {
    adjustment_interval = 10;
  } else if (stability_score < 4) {
    adjustment_interval = 5;
  } else if (stability_score < 7) {
    adjustment_interval = 2;
  } else {
    adjustment_interval = 1;
  }
  
  if (adjustment_counter > adjustment_interval) {
    Serial.println("Updating servos");
    adjustment_counter = 0;
    heat_dial_servo.set(heat_dial);
    flow_dial_servo.set(flow_dial);
  }
  
  Serial.print(millis());
  Serial.print(",");
  Serial.print(int_temp_lpf.getUnfiltered());
  Serial.print(",");
  Serial.print(int_temp_lpf.getLPF());
  Serial.print(",");
  Serial.print(int_temp_lpf.getDelta());
  Serial.print(",");
  Serial.print(vent_temp_lpf.getUnfiltered());
  Serial.print(",");
  Serial.print(vent_temp_lpf.getLPF());
  Serial.print(",");
  Serial.print(vent_temp_lpf.getDelta());
  Serial.print(",");
  Serial.print(stability_score);
  Serial.print(",");
  Serial.print(heat_dial);
  Serial.print(",");
  Serial.print(flow_dial);

  Serial.println();

  lcd.setCursor(0, 0);
  lcd.print("temp ");
  lcd.print(int(int_temp_lpf.getUnfiltered()));
  lcd.print("  vent ");
  lcd.print(int(vent_temp_lpf.getUnfiltered()));
  lcd.print("    ");

  lcd.setCursor(0, 1);
  lcd.print("heat ");
  lcd.print(int(heat_dial));
  lcd.print("  flow ");
  lcd.print(int(flow_dial));
  lcd.print("    ");

  delay(1 * 1000); // was 5 sec
}


