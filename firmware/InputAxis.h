#ifndef INPUTAXIS_H
#define INPUTAXIS_H

#include "AxisMap.h"
#include "EEPROMBuffer.h"
#include "Profiler.h"
#include <Arduino.h>
#include <stdint.h>

class InputAxis {
public:
  AxisMap axisMap;
  uint8_t deadzoneLow;
  uint8_t deadzoneHigh;
  uint8_t smoothing;
  uint8_t hysteresis;
  bool autoCalibrate;

  uint16_t historyLow;
  uint16_t historyHigh;

  InputAxis(uint8_t pin, bool invert);
  void update(uint16_t& output);
  void resetCalibration();
  void resetAll();
  void saveLoad(EEPROMBuffer& eeprom);

private:
  uint8_t const pin;
  bool const invert;

  Profiler profiler;
  uint16_t domainLow;
  uint16_t domainHigh;
  uint16_t domainReciprocal;
  uint16_t filtered;

  void updateDomain();
};

#endif  // ifndef INPUTAXIS_H