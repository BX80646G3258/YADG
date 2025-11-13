#include "MenuAdjust.h"
#include "Controller.h"
#include "Display.h"

using namespace Display;

namespace MenuAdjust {
namespace {
void draw(char const* const name, uint16_t value, uint16_t max_, uint16_t uiMax) {
  cls();
  print_P(name);
  if (max_ > 1) {
    setInvert(uiMax <= value || value <= 0);
    print(value, 1);
  } else {
    if (value > 0) {
      print_P(UIStrings::TRUE, 1);
    } else {
      print_P(UIStrings::FALSE, 1);
    }
  }
}
}  // namespace anon

void adjustU16(char const* const name, uint16_t& var, uint16_t max_, uint16_t uiMax) {
  uint16_t value = ((uint32_t)uiMax * var + max_ / 2) / max_;
  bool repaint = true;
  uint32_t holdTimestamp = millis();
  while (true) {
    controller.update();
    if (controller.state.down(ButtonMap::MENU_UP)) {
      holdTimestamp = millis();
      if (value < uiMax) {
        value++;
        repaint = true;
      }
    } else if (controller.state.down(ButtonMap::MENU_DOWN)) {
      holdTimestamp = millis();
      if (value > 0) {
        value--;
        repaint = true;
      }
    } else if (controller.state.down(ButtonMap::MENU_BACK)) {
      return;
    } else if (controller.state.down(ButtonMap::MENU_ENTER)) {
      var = ((uint32_t)value * max_ + uiMax / 2) / uiMax;
      return;
    }

    if (repaint) {
      draw(name, value, max_, uiMax);
      repaint = false;
    }

    if (millis() - holdTimestamp > HOLD_DELAY) {
      uint16_t newValue = value;
      while (controller.state.buttons[ButtonMap::MENU_UP] || controller.state.buttons[ButtonMap::MENU_DOWN]) {
        uint32_t elapsed = millis() - holdTimestamp - HOLD_DELAY;
        int32_t delta = ((uint64_t)elapsed * elapsed * HOLD_ACCEL / 2048 + elapsed * HOLD_SPEED) / 1024 + 1;
        if (!controller.state.buttons[ButtonMap::MENU_UP]) {
          delta = -delta;
        }
        newValue = min(max((int32_t)value + delta, 0), (int32_t)uiMax);
        draw(name, newValue, max_, uiMax);
        delay(10);
        controller.update();
      }
      value = newValue;
    }
  }
}

}  // namespace MenuAdjust