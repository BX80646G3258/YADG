#ifndef MENUADJUST_H
#define MENUADJUST_H

#include "ButtonMap.h"
#include "UIStrings.h"
#include <stdint.h>

namespace MenuAdjust {
static uint16_t const HOLD_DELAY = 500;
static uint8_t const HOLD_SPEED = 5;
static uint8_t const HOLD_ACCEL = 13;

void adjustU16(char const* const name, uint16_t& var, uint16_t max_, uint16_t uiMax);

template<typename T> void adjust(char const* const name, T& var, T max_, T uiMax) {
  // stuffing everything into the uint16_t function instead of proper templates saves flash memory
  uint16_t varU16 = var;
  adjustU16(name, varU16, max_, uiMax);
  var = varU16;
}

template<> inline void adjust<bool>(char const* const name, bool& var, bool max_, bool uiMax) {
  adjust<uint8_t>(name, (uint8_t&)var, max_, uiMax);
}
}  // namespace MenuAdjust

#endif  // ifndef MENUADJUST_H