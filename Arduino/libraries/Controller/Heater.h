#ifndef Heater_H
#define Heater_H

#include <DallasTemperature.h>
#include <Limits.h>
#include <OneWire.h>
#include <Servo.h>
#include <Time.h>
#include <Wire.h>
#include "AnalogButtons.h"
#include "DialServo.h"
#include "LowPassFilter.h"

// Pin layout
#define HEAT_SERVO_PIN 9
#define FLOW_SERVO_PIN 10
#define ONE_WIRE_BUS 8
#define TEMPERATURE_PRECISION 12

// Dial settings
#define HEAT_CLICKS { 0, 7, 17, 27, 39, 53, 65, 77, 90, 103, 115, 128, 139, 150 }
#define FLOW_CLICKS { 10, 37, 48, 63, 77, 89, 101, 114, 131, 145 }
#define MIN_HEAT 0
#define MAX_HEAT 13
#define MIN_FLOW 0
#define MAX_FLOW 9
#define MIN_HEAT_DIAL MIN_HEAT - MIN_FLOW - 2
#define MAX_HEAT_DIAL MAX_HEAT + MAX_FLOW + 2


class Heater {
  private:
    float temp_target;
    int max_flow;
    int adjustment_counter;
    float stability_score;

    float heat_dial;
    float flow_dial;

    OneWire oneWire;
    DallasTemperature sensors;
    DialServo heat_dial_servo;
    DialServo flow_dial_servo;
    LowPassFilter vent_temp_lpf;
    LowPassFilter int_temp_lpf;
    LowPassFilter heat_dial_lpf;
    LowPassFilter flow_dial_lpf;

    DeviceAddress ventThermometer, insideThermometer;

  public:
    Heater();
    void init();
    void flowOn();
    void flowOff();
    void updateDials(bool force);
    void traceToSerial();

    void setTempTarget(int temp);
    int getTempTarget();

    int getIntTemp();
    int getVentTemp();
    int getFlowDialPosition();
    int getHeatDialPosition();
};
#endif
