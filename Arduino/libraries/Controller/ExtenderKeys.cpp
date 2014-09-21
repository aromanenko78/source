#include "ExtenderKeys.h"

ExtenderKeys::ExtenderKeys(uint8_t extender_address) {
  this->extender_address = extender_address;
}

void ExtenderKeys::begin() {
  Serial.println("begin");
  extenderWrite(MCP23017_IOCON, 0b00000000); // do not mirror interrupts, enable sequential mode
  extenderWrite(MCP23017_GPPUA, 0xFF); // enable pull-up
  extenderWrite(MCP23017_IPOLA, 0xFF); // invert polarity
  extenderWrite(MCP23017_GPINTENA, 0xFF); // enable all interrupts
  extenderWrite(MCP23017_INTCONA, 0x00); // interrupt on pin state change
  // read from interrupt capture to clear
  extenderRead(MCP23017_INTCAPA);
  //attachInterrupt(0, onInterrupt, FALLING);

  Serial.println("begin end");
}

void ExtenderKeys::attachKeyHandler(uint8_t key, KeyHandler handler) {
  Serial.println("attach");
  handlers[key] = handler;
}

void ExtenderKeys::processKeys() {
  Serial.println("processKeys");
  // check interrupts
  if (interrupt) {
    Serial.println("processKeys2");
    uint8_t new_state = extenderRead(MCP23017_INTCAPA);
    interrupt = false;
    uint8_t changed = state ^ new_state;
    uint8_t bit_mask = 1;
    for (uint8_t i = 0; i < 8; i++) {
      if ((handlers[i]) &&
          (new_state & bit_mask) && 
          (changed & bit_mask)) {
        handlers[i]();
      }
      bit_mask <<= 1;
    }
  }
}

void ExtenderKeys::processInterrupt() {
  Serial.println("interrupt = true");
  interrupt = true;
}

/* ---- private helpers ---- */

void ExtenderKeys::extenderWrite(const uint8_t reg, const uint8_t data) {
  Wire.beginTransmission(extender_address);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t ExtenderKeys::extenderRead(const uint8_t reg) {
  Wire.beginTransmission(extender_address);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(extender_address, (uint8_t)1);
  return Wire.read();
}
