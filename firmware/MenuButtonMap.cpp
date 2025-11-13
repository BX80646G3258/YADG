#include "MenuButtonMap.h"
#include "ButtonMap.h"
#include "Controller.h"
#include "UIStrings.h"
#include "Display.h"

using namespace Display;
using namespace ButtonMap;
using namespace UIStrings;

namespace MenuButtonMap {

namespace {
const uint8_t COUNT = sizeof(BUTTON_ORDER) - 1;  // don't allow CONF button to be remapped

void move(uint8_t& selected, uint8_t& scroll, int8_t step) {
  if (-step > selected) {
    selected += COUNT;
  } else if (step > COUNT - selected - 1) {
    selected -= COUNT;
  }

  selected += step;

  if (selected < scroll)
    scroll = selected;
  if (selected >= scroll + ROWS) {
    scroll = selected - ROWS + 1;
  }
}

char const* getName(uint8_t button) {
  return pgm_read_byte_near(BUTTON_NAMES + button);
}

void draw(uint8_t selected, uint8_t scroll, bool drawPair) {
  cls();
  for (uint8_t i = scroll; i < COUNT && i < scroll + ROWS; i++) {
    uint8_t const row = i - scroll;
    uint8_t hwButton = pgm_read_byte_near(BUTTON_ORDER + i);

    setInvert(i == selected);

    char const* const buttonName = getName(pgm_read_byte_near(DEFAULT_BUTTON_MAP + hwButton));
    print_P(buttonName, row);

    if (drawPair) {
      for (uint8_t i = strlen_P(buttonName); i < COLS / 2; i++) {
        print(" ", row, i);
      }
      char const* mapName = getName(controller.buttonMap[hwButton]);
      print_P(mapName, row, COLS / 2);
    }
  }

  setInvert(false);
  if (scroll > 0) {
    print("#", COLS - 1, 0);  // # is replaced with up arrow in ModifiedFont.cpp
  }
  if (COUNT - scroll > ROWS) {
    print("$", COLS - 1, ROWS - 1);  // $ is replaced with down arrow in ModifiedFont.cpp
  }
}

uint8_t select() {
  uint8_t selected = 0;
  uint8_t scroll = 0;
  bool repaint = true;
  while (true) {
    controller.update();
    if (controller.state.down(MENU_UP)) {
      move(selected, scroll, -1);
      repaint = true;
    } else if (controller.state.down(MENU_DOWN)) {
      move(selected, scroll, 1);
      repaint = true;
    } else if (controller.state.down(MENU_BACK)) {
      return UINT8_MAX;
    } else if (controller.state.down(MENU_ENTER)) {
      return pgm_read_byte_near(DEFAULT_BUTTON_MAP + pgm_read_byte_near(BUTTON_ORDER + selected));
    }

    if (repaint) {
      draw(selected, scroll, false);
      repaint = false;
    }
  }
}
}  // namespace anon

void buttonMap() {
  uint8_t selected = 0;
  uint8_t scroll = 0;
  bool repaint = true;
  while (true) {
    controller.update();
    if (controller.state.down(MENU_UP)) {
      move(selected, scroll, -1);
      repaint = true;
    } else if (controller.state.down(MENU_DOWN)) {
      move(selected, scroll, 1);
      repaint = true;
    } else if (controller.state.down(MENU_BACK)) {
      return;
    } else if (controller.state.down(MENU_ENTER)) {
      uint8_t newMapping = select();
      if (newMapping != UINT8_MAX) {
        controller.buttonMap[pgm_read_byte_near(BUTTON_ORDER + selected)] = newMapping;
      }
      repaint = true;
    }

    if (repaint) {
      draw(selected, scroll, true);
      repaint = false;
    }
  }
}
}  // namespace MenuButtonMap