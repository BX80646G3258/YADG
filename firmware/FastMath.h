#ifndef FASTMATH_H
#define FASTMATH_H

#include <stdint.h>

namespace FastMath {
uint8_t sqrt16(uint16_t val);
uint16_t sqrt32(uint32_t val);
uint16_t sqrt32Approx(uint32_t val);

// output 0-2048
uint16_t atan2Fast(int16_t y, int16_t x);
// input 0-2048
int16_t sinFast(uint16_t x);

int16_t sLerpFast(int16_t a, int16_t b, uint8_t pct);
uint16_t uLerpFast(uint16_t a, uint16_t b, uint8_t pct);
}  // namespace FastMath

#endif  // ifndef FASTMATH_H