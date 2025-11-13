#ifndef DEBOUNCER_H
#define DEBOUNCER_H

#include "EEPROMBuffer.h"
#include <stdint.h>

class Debouncer {
public:
  // reject random signals before button press
  uint8_t msRiseDelay;
  // reject bounces after button press
  uint8_t msRiseTimeout;
  // reject random signals before button release
  uint8_t msFallDelay;
  // reject bounces after button release
  uint8_t msFallTimeout;

  Debouncer();
  bool Update(bool rawState, uint32_t timestamp);
  void resetAll();
  void saveLoad(EEPROMBuffer& eeprom);

private:
  uint32_t lastChangeTime;
  uint32_t lastRawChangeTime;
  uint32_t delay;
  uint32_t timeout;
  bool delayFinished;
  bool timeoutFinished;
  bool state;
  bool lastRawState;
};

#endif // ifndef DEBOUNCER_H