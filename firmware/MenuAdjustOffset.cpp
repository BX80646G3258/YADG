#include "Controller.h"
#include "Display.h"
#include "ButtonMap.h"
#include "FastMath.h"

using namespace ButtonMap;
using namespace Display;

namespace MenuAdjustOffset {

namespace {
uint8_t const SEGMENTS = 8;
uint8_t const RADIUS = min(WIDTH, HEIGHT) / 2;

void toScreen(uint16_t angle, uint8_t& x, uint8_t& y) {
  y = RADIUS - (int32_t)FastMath::sinFast(angle + 512) * RADIUS / INT16_MAX;
  x = (int32_t)FastMath::sinFast(angle + 1024) * RADIUS / INT16_MAX + RADIUS;
}

void drawCircle() {
  uint8_t x0, y0;
  toScreen(0, x0, y0);
  for (uint8_t i = 1; i < SEGMENTS + 1; i++) {
    uint16_t angle = (uint32_t)i * 2048 / SEGMENTS;
    uint8_t x1, y1;
    toScreen(angle, x1, y1);
    line(x0, y0, x1, y1);
    x0 = x1;
    y0 = y1;
  }
}

void draw(uint16_t angle) {
  cls();
  drawCircle();

  uint8_t x, y;
  toScreen(angle, x, y);
  line(RADIUS, RADIUS, x, y);

  print((uint32_t)angle * 360 / 2048, 1, (uint16_t)RADIUS * 2 / FONT_WIDTH + 2);
}
}  // namespace anon

void adjustOffset(InputAxisPolar& axis) {
  bool cachedPolar = axis.polarMode;
  axis.polarMode = false;
  uint8_t cachedSmoothingX = axis.smoothingX;
  axis.smoothingX = 0;
  uint8_t cachedSmoothingY = axis.smoothingY;
  axis.smoothingY = 0;
  uint16_t cachedOffset = axis.offset;
  axis.offset = false;

  draw(cachedOffset);

  uint16_t offset = cachedOffset;
  while (true) {
    controller.update();
    int16_t x, y;
    axis.update(x, y);
    uint16_t offsetRaw = (FastMath::atan2Fast(y, x) - 512) & 2047;

    if ((x != 0 || y != 0) && offsetRaw != offset) {
      offset = offsetRaw;
      draw(offset);
    }

    if (controller.state.down(HW_A)) {
      axis.offset = offset;

      axis.polarMode = cachedPolar;
      axis.smoothingX = cachedSmoothingX;
      axis.smoothingY = cachedSmoothingY;
      return;
    }
    if (controller.state.down(HW_B)) {
      axis.offset = cachedOffset;

      axis.polarMode = cachedPolar;
      axis.smoothingX = cachedSmoothingX;
      axis.smoothingY = cachedSmoothingY;
      return;
    }
  }
}

}  // namespace MenuAdjustOffset