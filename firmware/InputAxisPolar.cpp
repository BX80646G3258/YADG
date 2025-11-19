#include "InputAxisPolar.h"
#include "FastMath.h"
#include <Arduino.h>

using namespace FastMath;

InputAxisPolar::InputAxisPolar(uint8_t pinX, uint8_t pinY, bool invertX, bool invertY)
  : pinX(pinX), pinY(pinY), invertX(invertX), invertY(invertY), profiler(false) {

  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);

  resetAll();
}


void InputAxisPolar::calculateDomain(uint16_t& domainLow, uint16_t& domainHigh, uint8_t section) {
  domainLow = (uint32_t)deadzoneLow * history[section] >> 8;
  domainHigh = (uint32_t)deadzoneHigh * history[section] >> 8;
}

void InputAxisPolar::updateReciprocal(uint8_t section) {
  uint16_t domainLow, domainHigh;
  calculateDomain(domainLow, domainHigh, section);
  reciprocals[section] = ((uint32_t)1 << 16) / (domainHigh - domainLow);
}

void InputAxisPolar::update(int16_t& outX, int16_t& outY) {
  profiler.sample("read");
  uint16_t rawX = analogRead(pinX);
  uint16_t rawY = analogRead(pinY);

  profiler.sample("fltr");
  if (rawX > filteredX + hysteresis) {
    filteredX = rawX - hysteresis;
  } else if (rawX < filteredX - hysteresis) {
    filteredX = rawX + hysteresis;
  }

  if (rawY > filteredY + hysteresis) {
    filteredY = rawY - hysteresis;
  } else if (rawY < filteredY - hysteresis) {
    filteredY = rawY + hysteresis;
  }

  int16_t x = (int32_t)filteredX - centerX;
  int16_t y = (int32_t)filteredY - centerY;

  if (invertX) {
    x = -x;
  }
  if (invertY) {
    y = -y;
  }

  profiler.sample("atan");
  uint16_t theta = atan2Fast(y, x);

  profiler.sample("sqrt");
  // uint16_t r = sqrt32Approx((uint32_t)x * x + (uint32_t)y * y);  // 13 - 22 us
  uint16_t r = sqrt32((uint32_t)x * x + (uint32_t)y * y);  // 13 - 27 us
  // uint16_t r = sqrt((uint32_t)x * x + (uint32_t)y * y);  // 44 us

  profiler.sample("limt");
  uint8_t section = theta * RESOLUTION / 2048;
  theta = (theta - offset) & 2047;

  if (section >= RESOLUTION) {
    section = RESOLUTION - 1;
  }

  if (r > history[section]) {
    if (autoCalibrate) {
      history[section] = r;
      updateReciprocal(section);
    }
    r = UINT16_MAX;
  } else {
    uint16_t domainLow, domainHigh;
    calculateDomain(domainLow, domainHigh, section);
    if (r <= domainLow) {
      rotations = 0;
      lastTheta = 512;
      r = 0;
    } else if (r >= domainHigh) {
      r = UINT16_MAX;
    } else {
      r = (r - domainLow) * reciprocals[section];
    }
  }

  profiler.sample("writ");
  int16_t newX, newY;

  if (polarMode) {
    int16_t delta = theta - lastTheta;
    lastTheta = theta;
    if (delta > 1024) {
      rotations--;
    } else if (delta < -1024) {
      rotations++;
    }

    int16_t wrappedTheta = (int16_t)rotations * 2048 + (int16_t)theta - 512;

    if (wrappedTheta > (int16_t)maxTheta + 2048) {
      rotations--;
    } else if (-wrappedTheta > (int16_t)maxTheta + 2048) {
      rotations++;
    }

    wrappedTheta = -min(max(wrappedTheta, (int16_t)-maxTheta), (int16_t)maxTheta);
    r /= 2;
    newX = (int32_t)r * wrappedTheta / maxTheta;
    newY = r;
  } else {
    newX = (int32_t)r * sinFast(theta + 512) >> 16;
    newY = (int32_t)r * sinFast(theta) >> 16;
  }

  if (smoothingX == 0) {
    outX = newX;
  } else {
    outX = sLerpFast(newX, outX, 222 + smoothingX);
  }
  if (smoothingY == 0) {
    outY = newY;
  } else {
    outY = sLerpFast(newY, outY, 222 + smoothingY);
  }

  profiler.end();
}

void InputAxisPolar::resetCalibration() {
  for (uint8_t i = 0; i < RESOLUTION; i++) {
    history[i] = 0;
    reciprocals[i] = 0;
  }

  centerX = filteredX = analogRead(pinX);
  centerY = filteredY = analogRead(pinY);
}

void InputAxisPolar::resetAll() {
  deadzoneLow = .05 * UINT8_MAX;
  deadzoneHigh = .95 * UINT8_MAX;
  smoothingX = 0;
  smoothingY = 0;
  offset = 0;
  maxTheta = 135.0 * 2048 / 360;
  hysteresis = 2;
  autoCalibrate = true;
  polarMode = false;

  resetCalibration();
}

void InputAxisPolar::saveLoad(EEPROMBuffer& eeprom) {
  eeprom.readWrite(deadzoneLow);
  eeprom.readWrite(deadzoneHigh);
  eeprom.readWrite(smoothingX);
  eeprom.readWrite(smoothingY);
  eeprom.readWrite(offset);
  eeprom.readWrite(maxTheta);
  eeprom.readWrite(hysteresis);
  eeprom.readWrite(autoCalibrate);
  eeprom.readWrite(polarMode);
  eeprom.readWrite(centerX);
  eeprom.readWrite(centerY);
  for (uint8_t i = 0; i < RESOLUTION; i++) {
    eeprom.readWrite(history[i]);
    updateReciprocal(i);
  }
}