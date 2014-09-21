#include <OneWire.h>
#include <DallasTemperature.h>
#include <Time.h>

#define ONE_WIRE_BUS 8
#define TEMPERATURE_PRECISION 12

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress ventThermometer, insideThermometer;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("begin");

  sensors.begin();

  Serial.print("Device count: ");
  Serial.println(sensors.getDeviceCount());

  if (!sensors.getAddress(ventThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  if (!sensors.getAddress(insideThermometer, 1)) Serial.println("Unable to find address for Device 1"); 

  sensors.setResolution(ventThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
}

void loop(void)
{ 
  sensors.requestTemperatures();

  Serial.print("Vent: ");
  Serial.print(sensors.getTempC(ventThermometer));
  Serial.print(", inside: ");
  Serial.print(sensors.getTempC(insideThermometer));
  Serial.print(" time: ");
  Serial.print(now());
  Serial.println();

  delay(2000);
}

