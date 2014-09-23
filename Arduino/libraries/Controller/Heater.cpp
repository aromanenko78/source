#include "Heater.h"

Heater::Heater() : 
    temp_target(0),
    max_flow(MAX_FLOW),
    adjustment_counter(0),
    oneWire(ONE_WIRE_BUS),
    sensors(&oneWire),
    heat_dial_servo((int[])HEAT_CLICKS, MIN_HEAT + .5, MAX_HEAT, HEAT_SERVO_PIN),
    flow_dial_servo((int[])FLOW_CLICKS, MIN_FLOW + 1.5, MAX_FLOW, FLOW_SERVO_PIN),
    vent_temp_lpf(.1),
    int_temp_lpf(.05),
    heat_dial_lpf(.08),
    flow_dial_lpf(.08) {
}

void Heater::init() {
  flow_dial_servo.forceSet(0);

  delay(3 * 1000);

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

void Heater::setTempTarget(int temp) {
  temp_target = float(temp) + .5;
  updateDials(true);
}

int Heater::getTempTarget() {
  return int(temp_target - .5);
}

void Heater::flowOn() {
  max_flow = MAX_FLOW;
  updateDials(true);
}

void Heater::flowOff() {
  max_flow = MIN_FLOW;
  updateDials(true);
}

void Heater::updateDials(bool force) {
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
  heat_dial = heat_dial_lpf.getUnfiltered();
  
  flow_dial = 3;

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

  // hard limit flow dial
  if (flow_dial > max_flow) {
    flow_dial = max_flow;
  }

  // stability score, the lower the more stable
  stability_score = abs(vent_temp_lpf.getError()) + 2 * abs(vent_temp_lpf.getDelta()); // + abs(int_temp_lpf.getError());
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
}


int Heater::getIntTemp() {
  return int(int_temp_lpf.getUnfiltered());
}

int Heater::getVentTemp() {
  return int(vent_temp_lpf.getUnfiltered());
}

int Heater::getFlowDialPosition() {
  return int(flow_dial);
}

int Heater::getHeatDialPosition() {
  return int(heat_dial);
}

void Heater::traceToSerial() {
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
}
