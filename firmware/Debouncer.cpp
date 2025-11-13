#include "Debouncer.h"
#include <Arduino.h>

Debouncer::Debouncer() {
  resetAll();
}

bool Debouncer::Update(bool rawState, uint32_t timestamp) {

  if (rawState != lastRawState) {
    lastRawState = rawState;
    if (rawState == state) {
      delayFinished = true;
    } else {
      lastRawChangeTime = timestamp;
      delayFinished = false;
    }
    return state;
  }

  if (!timeoutFinished) {
    if (timestamp - lastChangeTime < timeout) {
      return state;
    } else {
      timeoutFinished = true;
    }
  }

  if (!delayFinished && timestamp - lastRawChangeTime > delay) {
    delayFinished = true;
    timeoutFinished = false;
    state = rawState;
    lastChangeTime = timestamp;
    if (state) {
      delay = msFallDelay;
      timeout = msRiseTimeout;
    } else {
      delay = msRiseDelay;
      timeout = msFallTimeout;
    }
    delay *= 1000;
    timeout *= 1000;
  }

  return state;
}

void Debouncer::resetAll() {
  msRiseDelay = 0;
  msRiseTimeout = 10;
  msFallDelay = 3;
  msFallTimeout = 1;

  lastChangeTime = lastRawChangeTime = micros();
  state = lastRawState = false;
  delay = msRiseDelay * 1000;
  delayFinished = false;
  timeout = 0;
  timeoutFinished = true;
}

void Debouncer::saveLoad(EEPROMBuffer& eeprom) {
  eeprom.readWrite(msRiseDelay);
  eeprom.readWrite(msRiseTimeout);
  eeprom.readWrite(msFallDelay);
  eeprom.readWrite(msFallTimeout);
}