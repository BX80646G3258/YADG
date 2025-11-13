#include "InputAxis.h"
#include "FastMath.h"

InputAxis::InputAxis(uint8_t pin, bool invert)
  : pin(pin), invert(invert), profiler(false) {
  pinMode(pin, INPUT);
  resetAll();
}

void InputAxis::updateDomain() {
  uint16_t inputRange = historyHigh - historyLow;
  domainLow = historyLow + ((uint32_t)inputRange * deadzoneLow >> 8);
  domainHigh = historyLow + ((int32_t)inputRange * deadzoneHigh >> 8);
  domainReciprocal = ((uint32_t)1 << 16) / (domainHigh - domainLow);
}

void InputAxis::update(uint16_t& output) {
  profiler.sample("read");
  uint16_t raw = analogRead(pin);

  profiler.sample("fltr");
  if (raw > filtered + hysteresis) {
    filtered = raw - hysteresis;
  } else if (raw + hysteresis < filtered) {
    filtered = raw + hysteresis;
  }

  uint16_t axis = invert ? UINT16_MAX - filtered : filtered;

  profiler.sample("acal");
  if (autoCalibrate) {
    if (axis < historyLow) {
      historyLow = axis;
      updateDomain();
    }
    if (axis > historyHigh) {
      historyHigh = axis;
      updateDomain();
    }
  }

  profiler.sample("map");

  uint16_t linear;
  if (axis <= domainLow) {
    linear = 0;
  } else if (axis >= domainHigh) {
    linear = UINT16_MAX;
  } else {
    linear = (axis - domainLow) * domainReciprocal;
  }
  profiler.sample("writ");

  uint16_t mapped = axisMap.map(linear);
  if (smoothing == 0) {
    output = mapped;
  } else {
    output = FastMath::uLerpFast(mapped, output, 222 + smoothing);
  }
  profiler.end();
}

void InputAxis::resetCalibration() {
  historyLow = UINT16_MAX;
  historyHigh = 0;
  updateDomain();

  filtered = analogRead(pin);
}

void InputAxis::resetAll() {
  axisMap.resetAll();
  deadzoneLow = .05 * UINT8_MAX;
  deadzoneHigh = .95 * UINT8_MAX;
  smoothing = 0;
  hysteresis = 1;

  autoCalibrate = true;
  resetCalibration();
}

void InputAxis::saveLoad(EEPROMBuffer& eeprom) {
  axisMap.saveLoad(eeprom);
  eeprom.readWrite(deadzoneLow);
  eeprom.readWrite(deadzoneHigh);
  eeprom.readWrite(smoothing);
  eeprom.readWrite(hysteresis);
  eeprom.readWrite(autoCalibrate);
  eeprom.readWrite(historyLow);
  eeprom.readWrite(historyHigh);
  updateDomain();
}