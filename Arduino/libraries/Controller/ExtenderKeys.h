#ifndef ExtenderKeys_H
#define ExtenderKeys_H

#include <Arduino.h>
#include <Wire.h>

// MCP23017      Arduino
// 12 (white) -> A5
// 13 (green) -> A4
// 19 (blue)  -> 2 (int0)

#define MCP23017_IODIRA   0x00
#define MCP23017_IPOLA    0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA  0x06
#define MCP23017_INTCONA  0x08
#define MCP23017_IOCON    0x0A
#define MCP23017_GPPUA    0x0C
#define MCP23017_INTFA    0x0E
#define MCP23017_INTCAPA  0x10
#define MCP23017_INTCAPB  0x11
#define MCP23017_GPIOA    0x12

class ExtenderKeys {
 private:
  typedef void(*KeyHandler)();
  uint8_t extender_address;
  uint8_t state;
  KeyHandler handlers[8];
  volatile bool interrupt;

  void onInterrupt();
  uint8_t extenderRead(uint8_t reg);
  void extenderWrite(uint8_t reg, uint8_t data);

 public:
  ExtenderKeys(uint8_t extender_address);
  void begin();
  void attachKeyHandler(uint8_t key, KeyHandler handler);  
  void processKeys();
  void processInterrupt();
};
#endif
