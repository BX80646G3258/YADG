#ifndef BUTTONMAP_H
#define BUTTONMAP_H

#include <XInput.h>

namespace ButtonMap {

// HW button to button matrix index
enum HardwareButton {
  HW_A = 1,
  HW_B = 2,
  HW_X = 0,
  HW_Y = 3,
  HW_UP = 10,
  HW_DOWN = 9,
  HW_LEFT = 11,
  HW_RIGHT = 8,
  HW_START = 4,
  HW_BACK = 7,
  HW_LOGO = 6,
  HW_CONF = 5,
  HW_LB = 14,
  HW_RB = 13,
  HW_R3 = 12,
  HW_L3 = 15,
};

// non-configurable buttons for menu navigation
enum MenuButton {
  MENU_UP = HW_UP,
  MENU_DOWN = HW_DOWN,
  MENU_LEFT = HW_LEFT,
  MENU_RIGHT = HW_RIGHT,
  MENU_ENTER = HW_A,
  MENU_BACK = HW_B
};

// HW button to XInput
uint8_t const DEFAULT_BUTTON_MAP[16] PROGMEM = {
  BUTTON_X,
  BUTTON_A,
  BUTTON_B,
  BUTTON_Y,
  BUTTON_START,
  15,  // HW_CONF
  BUTTON_LOGO,
  BUTTON_BACK,
  DPAD_RIGHT,
  DPAD_DOWN,
  DPAD_UP,
  DPAD_LEFT,
  BUTTON_R3,
  BUTTON_RB,
  BUTTON_LB,
  BUTTON_L3
};

}  // namespace ButtonMap

#endif // ifndef BUTTONMAP_H