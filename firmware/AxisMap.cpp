#include "AxisMap.h"
#include <Arduino.h>

AxisMap::AxisMap() {
  resetAll();
}

uint16_t AxisMap::map(uint16_t x) {
  lastInput = x;
  if (!enabled) {
    return x;
  }

  // lazy fix for full pressed case
  if (x == UINT16_MAX) {
    return table[RESOLUTION] * 257;
  }

  const uint16_t sectionSize = ((uint32_t)1 << 16) / RESOLUTION;
  uint8_t section = x / sectionSize;
  if (table[section + 1] > table[section]) {
    return ((uint32_t)(x - section * sectionSize) * (table[section + 1] - table[section]) / sectionSize + table[section]) * 257;
  } else {
    return (table[section] - (uint32_t)(x - section * sectionSize) * (table[section] - table[section + 1]) / sectionSize) * 257;
  }
}

uint16_t AxisMap::getLastInput() {
  return lastInput;
}

void AxisMap::resetAll() {
  enabled = false;
  for (uint8_t i = 0; i < sizeof(table) / sizeof(table[0]); i++) {
    table[i] = (uint16_t)i * UINT8_MAX / RESOLUTION;
  }
}

void AxisMap::saveLoad(EEPROMBuffer& eeprom) {
  eeprom.readWrite(enabled);
  for (uint8_t i = 0; i < sizeof(table) / sizeof(table[0]); i++) {
    eeprom.readWrite(table[i]);
  }
}
