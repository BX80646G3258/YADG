#ifndef UISTRINGS_H
#define UISTRINGS_H

#include "ButtonMap.h"

namespace UIStrings {

char const STARTUP[] PROGMEM = "yet another";
char const STARTUP2[] PROGMEM = "dogshit gamepad";

char const TRUE[] PROGMEM = "true";
char const FALSE[] PROGMEM = "false";

char const SAVE[] PROGMEM = "save";
char const LOAD[] PROGMEM = "load";
char const NO_DATA[] PROGMEM = "no data";
char const RESET_ALL[] PROGMEM = "reset all";
char const RESET_CALIBRATION[] PROGMEM = "reset calibration";
char const DONE[] PROGMEM = "done";
char const WORKING[] PROGMEM = "working...";
char const ENABLED[] PROGMEM = "enabled";
char const EDIT[] PROGMEM = "edit";

char const CONFIRM[] PROGMEM = "confirm factory reset";
char const RB_YES[] PROGMEM = "yes = RB";
char const LB_NO[] PROGMEM = "no  = LB";

char const HYSTERESIS[] PROGMEM = "hysteresis";
char const AUTOCAL[] PROGMEM = "auto calibrate";
char const DEADZONE_LOW[] PROGMEM = "deadzone low";
char const DEADZONE_HIGH[] PROGMEM = "deadzone high";
char const CURVE[] PROGMEM = "curve";

char const FACTORY_RESET[] PROGMEM = "factory reset";
char const CYCLE_POWER[] PROGMEM = "cycle power to finish";
char const SETTINGS[] PROGMEM = "settings";
char const PROFILES[] PROGMEM = "profiles";
char const DEFAULT_PROFILE[] PROGMEM = "default";
char const PROFILE_0[] PROGMEM = "profile 0";
char const PROFILE_1[] PROGMEM = "profile 1";
char const PROFILE_2[] PROGMEM = "profile 2";

char const JOYSTICK_LEFT[] PROGMEM = "joystick left";
char const JOYSTICK_RIGHT[] PROGMEM = "joystick right";
char const SMOOTHING_X[] PROGMEM = "smoothing x";
char const SMOOTHING_Y[] PROGMEM = "smoothing y";
char const OFFSET[] PROGMEM = "offset angle";
char const POLAR_MODE[] PROGMEM = "polar mode";
char const MAX_THETA[] PROGMEM = "max angle";

char const TRIGGER_LEFT[] PROGMEM = "trigger left";
char const TRIGGER_RIGHT[] PROGMEM = "trigger right";
char const SMOOTHING[] PROGMEM = "smoothing";

char const BUTTONS[] PROGMEM = "buttons";
char const BUTTON_MAP[] PROGMEM = "button map";
char const DEBOUNCE[] PROGMEM = "debounce";
char const RISE_DELAY[] PROGMEM = "rise delay";
char const RISE_TIMEOUT[] PROGMEM = "rise timeout";
char const FALL_DELAY[] PROGMEM = "fall delay";
char const FALL_TIMEOUT[] PROGMEM = "fall timeout";

char const A[] PROGMEM = "a";
char const B[] PROGMEM = "b";
char const X[] PROGMEM = "x";
char const Y[] PROGMEM = "y";
char const UP[] PROGMEM = "up";
char const DOWN[] PROGMEM = "down";
char const LEFT[] PROGMEM = "left";
char const RIGHT[] PROGMEM = "right";
char const START[] PROGMEM = "start";
char const BACK[] PROGMEM = "back";
char const LOGO[] PROGMEM = "logo";
char const CONF[] PROGMEM = "conf";
char const LB[] PROGMEM = "LB";
char const RB[] PROGMEM = "RB";
char const L3[] PROGMEM = "L3";
char const R3[] PROGMEM = "R3";

// XInput enum to button name
char const* const BUTTON_NAMES[] PROGMEM = {
  LOGO,
  A,
  B,
  X,
  Y,
  LB,
  RB,
  BACK,
  START,
  L3,
  R3,
  UP,
  DOWN,
  LEFT,
  RIGHT,
  CONF
};

// menu index to HW button
uint8_t const BUTTON_ORDER[] PROGMEM = {
  ButtonMap::HW_A,
  ButtonMap::HW_B,
  ButtonMap::HW_X,
  ButtonMap::HW_Y,
  ButtonMap::HW_UP,
  ButtonMap::HW_DOWN,
  ButtonMap::HW_LEFT,
  ButtonMap::HW_RIGHT,
  ButtonMap::HW_LOGO,
  ButtonMap::HW_START,
  ButtonMap::HW_BACK,
  ButtonMap::HW_LB,
  ButtonMap::HW_RB,
  ButtonMap::HW_L3,
  ButtonMap::HW_R3,
  ButtonMap::HW_CONF
};

}  // namespace UIStrings

#endif  // #ifndef UISTRINGS_H