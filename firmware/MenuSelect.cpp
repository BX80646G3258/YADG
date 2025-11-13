#include "MenuSelect.h"
#include "Controller.h"
#include "ButtonMap.h"
#include "Display.h"

using namespace ButtonMap;
using namespace Display;

namespace MenuSelect {
namespace {
void move(uint8_t& selected, uint8_t& scroll, uint8_t count, int8_t step) {
  if (-step > selected) {
    selected += count;
  } else if (step > count - selected - 1) {
    selected -= count;
  }

  selected += step;

  if (selected < scroll) {
    scroll = selected;
  }
  if (selected >= scroll + ROWS) {
    scroll = selected - ROWS + 1;
  }
}

void draw(char const* const names[], uint8_t selected, uint8_t scroll, uint8_t count) {
  cls();
  for (uint8_t i = scroll; i < count && i < scroll + ROWS; i++) {
    setInvert(i == selected);
    print_P(names[i], i - scroll);
  }

  setInvert(false);
  if (scroll > 0) {
    print("#", 0, COLS - 1);  // # is replaced with up arrow in ModifiedFont.cpp
  }
  if (count - scroll > ROWS) {
    print("$", ROWS - 1, COLS - 1);  // $ is replaced with down arrow in ModifiedFont.cpp
  }
}
}  // namespace anon

void select(void (*const entries[])(), char const* const names[], const uint8_t count) {
  uint8_t selected = 0;
  uint8_t scroll = 0;
  while (selectIndex(names, count, selected, scroll)) {
    entries[selected]();
  }
}

bool selectIndex(char const* const names[], const uint8_t count, uint8_t& selected, uint8_t& scroll) {
  bool repaint = true;
  while (true) {
    controller.update();
    if (controller.state.down(MENU_UP)) {
      move(selected, scroll, count, -1);
      repaint = true;
    } else if (controller.state.down(MENU_DOWN)) {
      move(selected, scroll, count, 1);
      repaint = true;
    } else if (controller.state.down(MENU_BACK)) {
      return false;
    } else if (controller.state.down(MENU_ENTER)) {
      return true;
    }

    if (repaint) {
      draw(names, selected, scroll, count);
      repaint = false;
    }
  }
}
}  // namespace MenuSelect