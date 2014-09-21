// mcp23017.ino
// 12 (white) -> A5
// 13 (green) -> A4
// 19 (blue)  -> 2 (int0)
#include "Wire.h"

#define MCPADDR  0x20

#define IODIRA   0x00
#define IPOLA    0x02
#define GPINTENA 0x04
#define DEFVALA  0x06
#define INTCONA  0x08
#define IOCON    0x0A
#define GPPUA    0x0C
#define INTFA    0x0E
#define INTCAPA  0x10
#define INTCAPB  0x11
#define GPIOA    0x12

void setup() {
  Serial.begin(9600);
  Wire.begin();

  //expanderWriteBoth(IOCON, 0b01000000); // mirror interrupts, enable sequential mode
  expanderWriteBoth(IOCON, 0b00000000); // do not mirror interrupts, enable sequential mode
  expanderWriteBoth(GPPUA, 0xFF); // enable pull-up
  expanderWriteBoth(IPOLA, 0xFF); // invert polarity
  expanderWriteBoth(GPINTENA, 0xFF); // enable all interrupts
  expanderWriteBoth(INTCONA, 0x00); // interrupt on pin state change
  
  // read from interrupt capture ports to clear them
  expanderReadBoth(INTCAPA);
  
  attachInterrupt(0, onInterrupt, FALLING);
}

volatile bool keypress = false;

void loop() {
  if (keypress) {
    keypress = false;
    Serial.print("keypress ");
    Serial.print("0x");
    Serial.println(expanderReadBoth(INTCAPA), HEX);
  }

  Serial.print("loop ");
  Serial.print("0x");
  Serial.println(expanderReadBoth(GPIOA), HEX);
  delay(2000);
}

void onInterrupt() {
  Serial.println("onInterrupt");
  keypress = true;
  //delay(100);
  //Serial.print(" 0x");
  //Serial.print(expanderReadBoth(GPIOA), HEX);
  //Serial.print(expanderReadBoth(INTFA), HEX);
  //Serial.print(expanderReadBoth(INTCAPA), HEX);
  //Serial.print(expanderRead(INTCAPA), HEX);
  //Serial.println();
}

void expanderWriteBoth(const byte reg, const byte data) {
  Wire.beginTransmission(MCPADDR);
  Wire.write(reg);
  Wire.write(data);  // port A
  Wire.write(data);  // port B
  Wire.endTransmission();
}

uint8_t expanderRead(const byte reg) {
  Wire.beginTransmission(MCPADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(MCPADDR, 2);
  return Wire.read();
}

uint16_t expanderReadBoth(const byte reg) {
  Wire.beginTransmission(MCPADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(MCPADDR, 2);
  uint8_t a = Wire.read();
  uint16_t ba = Wire.read();
  ba <<= 8;
  ba |= a;
  return ba;
}
