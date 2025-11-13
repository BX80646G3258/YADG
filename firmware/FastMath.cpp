#include "FastMath.h"

namespace FastMath {
uint8_t sqrt16(uint16_t val) {
  // __builtin_clz is undefined for 0
  if (val == 0) {
    return 0;
  }

  // approximate sqrt with 2 ^ (log2(v) / 2)
  uint8_t mask = ((uint16_t)1 << 7) >> (__builtin_clz(val) / 2);
  uint8_t guess = 0;

  // test each remaining bit
  do {
    uint8_t temp = guess | mask;
    if ((uint16_t)temp * temp <= val) {
      guess = temp;
    }
    mask >>= 1;
  } while (mask);

  // yippeee
  return guess;
}

namespace {
uint32_t rshift32(uint32_t val, uint8_t shift) {
  if (shift >= 32) {
    return 0;
  }

  switch (shift / 8) {
    case 0:
      break;
    case 1:
      val >>= 8;
      shift -= 8;
      break;
    case 2:
      val >>= 16;
      shift -= 16;
      break;
    case 3:
      val >>= 24;
      shift -= 24;
      break;
  }
  return val >> shift;
}

__attribute__((always_inline)) inline void sqrt32_part(uint32_t val, uint16_t& guess, uint8_t& mask) {
  // pack as many bits as possible into a 16 bit square root
  uint8_t shift = __builtin_clzl(val) / 2;
  guess = sqrt16(rshift32(val, 16 - shift * 2)) << (8 - shift);
  mask = 1 << (7 - shift);
}
} // namespace anon

uint16_t sqrt32Approx(uint32_t val) {
  // 16 bit square root is much much faster
  if (val <= UINT16_MAX) {
    return sqrt16(val);
  }

  uint16_t guess;
  uint8_t mask;
  sqrt32_part(val, guess, mask);
  return guess;
}

uint16_t sqrt32(uint32_t val) {
  // 16 bit square root is much much faster
  if (val <= UINT16_MAX) {
    return sqrt16(val);
  }

  uint16_t guess;
  uint8_t mask;

  if (val < (uint32_t)1 << 24) {
    sqrt32_part(val, guess, mask);
  } else {
    // overhead isn't worth the bit shifts
    guess = sqrt16(val >> 16) << 8;
    mask = 1 << 7;
  }

  // test each remaining bit
  while (mask) {
    uint16_t temp = guess | mask;
    if ((uint32_t)temp * temp <= val) {
      guess = temp;
    }
    mask >>= 1;
  }

  // yippeee
  return guess;
}

namespace {
uint8_t atanOctant(uint8_t x) {
  // return ((uint32_t)22642190 - (uint32_t)22742 * x) * x / 16777216; // 50.7 us
  return x;  // 40.2 us
  // lmfao
}

uint16_t atan2Quadrant(uint16_t y, uint16_t x) {
  if (y < x) {
    return atanOctant((uint32_t)UINT8_MAX * y / x);
  } else if (y > x) {
    return 512 - atanOctant((uint32_t)UINT8_MAX * x / y);
  } else {
    return 255;
  }
}
} // namespace anon

uint16_t atan2Fast(int16_t y, int16_t x) {
  if (y >= 0) {
    if (x >= 0) {
      return atan2Quadrant(y, x);
    } else {
      return 1024 - atan2Quadrant(y, -x);
    }
  } else {
    if (x >= 0) {
      return 2048 - atan2Quadrant(-y, x);
    } else {
      return 1024 + atan2Quadrant(-y, -x);
    }
  }
}

namespace {
uint16_t sinQuadrant(uint16_t x) {
  // bit shifts of multiples of 8 are significantly faster
  uint16_t temp = ((13446832 - (uint32_t)(2303 + (uint16_t)15 * x) * x) * x) >> 16;
  return temp >> 1;
}
} // namespace anon

int16_t sinFast(uint16_t x) {
  x &= 2047;
  switch (x / 512) {
    case 0:
      return sinQuadrant(x);
    case 1:
      return sinQuadrant(1024 - x);
    case 2:
      return -sinQuadrant(x - 1024);
    case 3:
      return -sinQuadrant(2048 - x);
  }
  return 0;
}

int16_t sLerpFast(int16_t a, int16_t b, uint8_t pct) {
  if (pct == 0) {
    return a;
  }
  if (pct == UINT8_MAX) {
    return b;
  }
  return (((int32_t)b * pct + (int32_t)a * (UINT8_MAX - pct)) * 257) >> 16;
}

uint16_t uLerpFast(uint16_t a, uint16_t b, uint8_t pct) {
  if (pct == 0) {
    return a;
  }
  if (pct == UINT8_MAX) {
    return b;
  }
  return (((uint32_t)b * pct + (uint32_t)a * (UINT8_MAX - pct)) * 257) >> 16;
}
} // namespace FastMath