#include "MenuAdjustAxisMap.h"
#include "Controller.h"
#include "Display.h"
#include "ButtonMap.h"

using namespace ButtonMap;
using namespace Display;

namespace MenuAdjustAxisMap {

namespace {
uint8_t const STEP = 256 / HEIGHT;
uint8_t const CURSOR_SIZE = 4;

void indexToScreen(uint8_t* table, uint8_t i, uint8_t& xOut, uint8_t& yOut) {
  xOut = (uint16_t)i * (WIDTH - 1) / AxisMap::RESOLUTION;
  yOut = (uint16_t)(UINT8_MAX - table[i]) * (HEIGHT - 1) / UINT8_MAX;
}

void drawCursor(uint8_t* table, uint8_t selected) {
  uint8_t x, y, y1, y2;
  indexToScreen(table, selected, x, y);
  if (y <= CURSOR_SIZE) {
    y1 = 0;
  } else {
    y1 = y - CURSOR_SIZE;
  }
  if (y > HEIGHT - CURSOR_SIZE - 1) {
    y2 = HEIGHT - 1;
  } else {
    y2 = y + CURSOR_SIZE;
  }
  line(x, y1, x, y2);

  uint8_t col;
  if (selected > AxisMap::RESOLUTION / 2) {
    col = 0;
  } else {
    col = COLS - 3;
  }
  print(table[selected] * 100 / 255, ROWS - 1, col);
}

void drawLive(uint8_t x) {
  line(x, 0, x, HEIGHT);
}

void draw(AxisMap& axisMap, uint8_t selected, uint8_t live) {
  cls();

  uint8_t x1, y1;
  indexToScreen(axisMap.table, 0, x1, y1);
  for (uint8_t i = 1; i < sizeof(axisMap.table) / sizeof(axisMap.table[0]); i++) {
    uint8_t x2, y2;
    indexToScreen(axisMap.table, i, x2, y2);
    line(x1, y1, x2, y2);
    x1 = x2;
    y1 = y2;
  }

  drawLive(live);
  drawCursor(axisMap.table, selected);
}
}  // namespace anon

void adjustAxisMap(AxisMap& axisMap) {
  uint8_t oldTable[sizeof(axisMap.table) / sizeof(axisMap.table[0])];
  memcpy(oldTable, axisMap.table, sizeof(oldTable));

  uint8_t selected = 0;
  uint8_t lastLive = 0;
  bool repaint = true;
  while (true) {
    controller.update();

    uint8_t liveValue = (uint32_t)axisMap.getLastInput() * WIDTH >> 16;
    if (liveValue != lastLive) {
      repaint = true;
    }
    lastLive = liveValue;

    if (controller.state.down(HW_LEFT)) {
      if (selected > 0) {
        selected--;
      }
      repaint = true;
    }
    if (controller.state.down(HW_RIGHT)) {
      if (selected < AxisMap::RESOLUTION) {
        selected++;
      }
      repaint = true;
    }
    if (controller.state.down(HW_UP)) {
      if (UINT8_MAX - axisMap.table[selected] >= STEP) {
        axisMap.table[selected] += STEP;
      } else {
        axisMap.table[selected] = UINT8_MAX;
      }
      repaint = true;
    }
    if (controller.state.down(HW_DOWN)) {
      if (axisMap.table[selected] >= STEP) {
        axisMap.table[selected] -= STEP;
      } else {
        axisMap.table[selected] = 0;
      }
      repaint = true;
    }
    if (controller.state.down(HW_A)) {
      return;
    }
    if (controller.state.down(HW_B)) {
      memcpy(axisMap.table, oldTable, sizeof(axisMap.table));
      return;
    }

    if (repaint) {
      draw(axisMap, selected, liveValue);
      repaint = false;
    }
  }
}
}  // namespace MenuAdjustAxisMap