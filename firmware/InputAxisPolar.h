#ifndef INPUTAXISPOLAR_H
#define INPUTAXISPOLAR_H

#include "EEPROMBuffer.h"
#include "Profiler.h"

#include <Arduino.h>
#include <stdint.h>

class InputAxisPolar {
public:
  static uint8_t const RESOLUTION = 32;

  uint8_t deadzoneLow;
  uint8_t deadzoneHigh;
  uint16_t centerX;
  uint16_t centerY;
  uint8_t smoothingX;
  uint8_t smoothingY;
  uint16_t offset;
  uint16_t maxTheta;
  uint8_t hysteresis;
  bool autoCalibrate;
  bool polarMode;

  uint16_t history[RESOLUTION];
  uint16_t reciprocals[RESOLUTION];
  uint16_t lastTheta;
  int8_t rotations;

  InputAxisPolar(uint8_t pinX, uint8_t pinY, bool invertX, bool invertY);
  void update(int16_t& outX, int16_t& outY);
  void resetCalibration();
  void resetAll();
  void saveLoad(EEPROMBuffer& eeprom);

private:
  uint8_t const pinX;
  uint8_t const pinY;
  bool const invertX;
  bool const invertY;

  uint16_t filteredX;
  uint16_t filteredY;

  Profiler profiler;

  void calculateDomain(uint16_t& domainLow, uint16_t& domainHigh, uint8_t section);
  void updateReciprocal(uint8_t section);
};

#endif  // ifndef INPUTAXISPOLAR_H