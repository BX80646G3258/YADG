#include "MenuSystem.h"

#include "MenuSelect.h"
#include "MenuAdjust.h"
#include "MenuButtonMap.h"
#include "MenuAdjustAxisMap.h"
#include "MenuAdjustOffset.h"
#include "Controller.h"
#include "UIStrings.h"
#include "Display.h"

using namespace UIStrings;
using namespace ButtonMap;
using namespace MenuAdjust;
using namespace MenuSelect;
using namespace MenuButtonMap;
using namespace MenuAdjustAxisMap;
using namespace MenuAdjustOffset;
using namespace Display;

namespace MenuSystem {

uint8_t const PROFILES_COUNT = 3;
uint16_t const MESSAGE_DELAY = 500;

namespace {

static void printWorking() {
  cls();
  print_P(WORKING);
}


static void printDone() {
  cls();
  print_P(DONE);
  delay(MESSAGE_DELAY);
}

#define ARRAY(...) \
  { __VA_ARGS__ }

#define SELECT(identifier, entries_, names_) \
  void identifier() { \
    void (*const entries[])() = entries_; \
    char const* const names[] = names_; \
    select(entries, names, sizeof(entries) / sizeof(entries[0])); \
  }

static void axisMap(AxisMap& axisMap) {
  uint8_t selected = 0;
  uint8_t scroll = 0;
  char const* const entries[] = { ENABLED, EDIT, RESET_ALL };
  while (selectIndex(entries, sizeof(entries) / sizeof(entries[0]), selected, scroll)) {
    switch (selected) {
      case 0:
        adjust<bool>(ENABLED, axisMap.enabled, 1, 1);
        break;
      case 1:
        adjustAxisMap(axisMap);
        break;
      case 2:
        printWorking();
        axisMap.resetAll();
        printDone();
        break;
      default:
        break;
    }
  }
}

static void adjustInputAxis(InputAxis& axis) {
  uint8_t selected = 0;
  uint8_t scroll = 0;
  char const* const entries[] = { AUTOCAL, DEADZONE_LOW, DEADZONE_HIGH, SMOOTHING, HYSTERESIS, CURVE, RESET_CALIBRATION, RESET_ALL };
  while (selectIndex(entries, sizeof(entries) / sizeof(entries[0]), selected, scroll)) {
    switch (selected) {
      case 0:
        adjust<bool>(AUTOCAL, axis.autoCalibrate, 1, 1);
        break;
      case 1:
        adjust<uint8_t>(DEADZONE_LOW, axis.deadzoneLow, UINT8_MAX, 100);
        break;
      case 2:
        adjust<uint8_t>(DEADZONE_HIGH, axis.deadzoneHigh, UINT8_MAX, 100);
        break;
      case 3:
        adjust<uint8_t>(SMOOTHING, axis.smoothing, 32, 32);
        break;
      case 4:
        adjust<uint8_t>(HYSTERESIS, axis.hysteresis, UINT8_MAX, UINT8_MAX);
        break;
      case 5:
        axisMap(axis.axisMap);
        break;
      case 6:
        printWorking();
        axis.resetCalibration();
        printDone();
        break;
      case 7:
        printWorking();
        axis.resetAll();
        printDone();
        break;
      default:
        break;
    }
  }
}

static void adjustInputAxisPolar(InputAxisPolar& axis) {
  uint8_t selected = 0;
  uint8_t scroll = 0;
  char const* const entries[] = { AUTOCAL, DEADZONE_LOW, DEADZONE_HIGH, SMOOTHING_X, SMOOTHING_Y, HYSTERESIS, POLAR_MODE, MAX_THETA, OFFSET, RESET_CALIBRATION, RESET_ALL };
  while (selectIndex(entries, sizeof(entries) / sizeof(entries[0]), selected, scroll)) {
    switch (selected) {
      case 0:
        adjust<bool>(AUTOCAL, axis.autoCalibrate, 1, 1);
        break;
      case 1:
        adjust<uint8_t>(DEADZONE_LOW, axis.deadzoneLow, UINT8_MAX, 100);
        break;
      case 2:
        adjust<uint8_t>(DEADZONE_HIGH, axis.deadzoneHigh, UINT8_MAX, 100);
        break;
      case 3:
        adjust<uint8_t>(SMOOTHING_X, axis.smoothingX, 32, 32);
        break;
      case 4:
        adjust<uint8_t>(SMOOTHING_Y, axis.smoothingY, 32, 32);
        break;
      case 5:
        adjust<uint8_t>(HYSTERESIS, axis.hysteresis, UINT8_MAX, UINT8_MAX);
        break;
      case 6:
        adjust<bool>(POLAR_MODE, axis.polarMode, 1, 1);
        break;
      case 7:
        adjust<uint16_t>(MAX_THETA, axis.maxTheta, 2048 * 8, 360 * 8);
        break;
      case 8:
        adjustOffset(axis);
        // adjust<uint16_t>(OFFSET, axis.offset,  2048, 360);
        break;
      case 9:
        printWorking();
        axis.resetCalibration();
        printDone();
        break;
      case 10:
        printWorking();
        axis.resetAll();
        printDone();
        break;
      default:
        break;
    }
  }
}

static void adjustDebouncer(Debouncer& debouncer) {
  uint8_t selected = 0;
  uint8_t scroll = 0;
  char const* const entries[] = { RISE_DELAY, RISE_TIMEOUT, FALL_DELAY, FALL_TIMEOUT, RESET_ALL };
  while (selectIndex(entries, sizeof(entries) / sizeof(entries[0]), selected, scroll)) {
    switch (selected) {
      case 0:
        adjust<uint8_t>(RISE_DELAY, debouncer.msRiseDelay, UINT8_MAX, UINT8_MAX);
        break;
      case 1:
        adjust<uint8_t>(RISE_TIMEOUT, debouncer.msRiseTimeout, UINT8_MAX, UINT8_MAX);
        break;
      case 2:
        adjust<uint8_t>(FALL_DELAY, debouncer.msFallDelay, UINT8_MAX, UINT8_MAX);
        break;
      case 3:
        adjust<uint8_t>(FALL_TIMEOUT, debouncer.msFallTimeout, UINT8_MAX, UINT8_MAX);
        break;
      case 4:
        printWorking();
        debouncer.resetAll();
        printDone();
        break;
      default:
        return;
    }
  }
}

static void debounce() {
  uint8_t const count = sizeof(BUTTON_ORDER);
  uint8_t selected = 0;
  uint8_t scroll = 0;
  char const* entries[count];
  for (uint8_t i = 0; i < count; i++) {
    entries[i] = pgm_read_word_near(BUTTON_NAMES + pgm_read_byte_near(DEFAULT_BUTTON_MAP + pgm_read_byte_near(BUTTON_ORDER + i)));
  }
  while (selectIndex(entries, count, selected, scroll)) {
    adjustDebouncer(controller.buttonMatrix.debouncers[pgm_read_byte_near(BUTTON_ORDER + selected)]);
  }
}

static void load(uint8_t index) {
  printWorking();
  uint16_t result = controller.saveLoad(index, EEPROMBuffer::READ);
  if (result == 0) {
    cls();
    print_P(NO_DATA);
    delay(MESSAGE_DELAY);
  } else {
    printDone();
  }
}

static void save(uint8_t index) {
  printWorking();
  uint16_t result = controller.saveLoad(index, EEPROMBuffer::WRITE);
  cls();
  print_P(DONE);
  print(result, ROWS - 1);
  delay(MESSAGE_DELAY);
}

static void adjustProfile(uint8_t index) {
  uint8_t selected = 0;
  uint8_t scroll = 0;
  char const* const entries[] = { LOAD, SAVE };
  while (selectIndex(entries, sizeof(entries) / sizeof(entries[0]), selected, scroll)) {
    switch (selected) {
      case 0:
        load(index);
        break;
      case 1:
        save(index);
        break;
      default:
        break;
    }
  }
}

static uint8_t getDefaultProfile() {
  uint8_t index = EEPROM.read(DEFAULT_PROFILE_ADDR);
  if (index >= PROFILES_COUNT) {
    index = 0;
  }
  return index;
}

static void adjustDefaultProfile() {
  uint8_t index = getDefaultProfile();
  adjust<uint8_t>(DEFAULT_PROFILE, index, PROFILES_COUNT - 1, PROFILES_COUNT - 1);
  EEPROM.update(DEFAULT_PROFILE_ADDR, index);
}

static void profiles() {
  uint8_t selected = 0;
  uint8_t scroll = 0;
  char const* const entries[] = { DEFAULT_PROFILE, PROFILE_0, PROFILE_1, PROFILE_2 };
  while (selectIndex(entries, sizeof(entries) / sizeof(entries[0]), selected, scroll)) {
    switch (selected) {
      case 0:
        adjustDefaultProfile();
        break;
      default:
        adjustProfile(selected - 1);
        break;
    }
  }
}

static void factoryReset() {
  cls();
  setInvert(true);
  print_P(CONFIRM);
  setInvert(false);
  print_P(RB_YES, 1);
  print_P(LB_NO, 2);

  while (true) {
    controller.update();
    if (controller.state.down(HW_LB)) {
      return;
    } else if (controller.state.down(HW_RB)) {
      break;
    }
  }

  printWorking();
  controller.factoryReset();
  cls();
  print_P(CYCLE_POWER);
  while (true) { delay(1); }
}

void buttons() {
  void (*const entries[])() = { buttonMap, debounce };
  char const* const names[] = { BUTTON_MAP, DEBOUNCE };
  select(entries, names, sizeof(entries) / sizeof(entries[0]));
}

void settings() {
  uint8_t selected = 0;
  uint8_t scroll = 0;
  char const* const entries[] = { BUTTONS, JOYSTICK_LEFT, JOYSTICK_RIGHT, UIStrings::TRIGGER_LEFT, UIStrings::TRIGGER_RIGHT, RESET_ALL };
  while (selectIndex(entries, sizeof(entries) / sizeof(entries[0]), selected, scroll)) {
    switch (selected) {
      case 0:
        buttons();
        break;
      case 1:
        adjustInputAxisPolar(controller.joystickL);
        break;
      case 2:
        adjustInputAxisPolar(controller.joystickR);
        break;
      case 3:
        adjustInputAxis(controller.triggerL);
        break;
      case 4:
        adjustInputAxis(controller.triggerR);
        break;
      case 5:
        printWorking();
        controller.resetAll();
        printDone();
        break;
    }
  }
}

void main() {
  void (*const entries[])() = { settings, profiles, factoryReset };
  char const* const names[] = { SETTINGS, PROFILES, FACTORY_RESET };
  select(entries, names, sizeof(entries) / sizeof(entries[0]));
}
}  // namespace anon

void run() {
  main();

  do {
    controller.update();
  } while (controller.state.buttons[HW_B]);

  cls();
}

void loadDefault() {
  controller.saveLoad(getDefaultProfile(), EEPROMBuffer::READ);
}
}  // namespace MenuSystem