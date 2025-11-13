#ifndef AXISMAP_H
#define AXISMAP_H

#include "EEPROMBuffer.h"
#include <stdint.h>

class AxisMap {
public:
  static uint8_t const RESOLUTION = 8;

  uint8_t table[RESOLUTION + 1];
  bool enabled;

  AxisMap();
  uint16_t map(uint16_t x);
  uint16_t getLastInput();
  void resetAll();
  void saveLoad(EEPROMBuffer& eeprom);
private:
  uint16_t lastInput;
};

#endif  // ifndef AXISMAP_H