#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "ButtonMatrix.h"
#include "InputAxis.h"
#include "InputAxisPolar.h"
#include "Profiler.h"
#include "ButtonMap.h"
#include "PinMap.h"


struct ControllerState {
  bool buttons[ButtonMatrix<MATRIX_ROW_COUNT, MATRIX_COL_COUNT>::BUTTON_COUNT];
  bool lastButtons[ButtonMatrix<MATRIX_ROW_COUNT, MATRIX_COL_COUNT>::BUTTON_COUNT];

  uint16_t triggerL;
  uint16_t triggerR;

  int16_t joystickLX;
  int16_t joystickLY;

  int16_t joystickRX;
  int16_t joystickRY;

  bool down(uint8_t i) {
    return buttons[i] && !lastButtons[i];
  }

  bool up(uint8_t i) {
    return !buttons[i] && lastButtons[i];
  }
};

class Controller {
public:
  static uint16_t const PROFILE_SIZE = 320;

  ButtonMatrix<MATRIX_ROW_COUNT, MATRIX_COL_COUNT>
    buttonMatrix;
  uint8_t buttonMap[sizeof(ButtonMap::DEFAULT_BUTTON_MAP)];

  InputAxis triggerL;
  InputAxis triggerR;

  InputAxisPolar joystickL;
  InputAxisPolar joystickR;

  ControllerState state;

  Controller();
  void begin();
  void update();
  uint8_t mapIndex(uint8_t i);
  void resetAll();
  void factoryReset();
  uint16_t saveLoad(uint8_t index, EEPROMBuffer::Mode mode);
private:
  Profiler profiler;
};

extern Controller controller;
#endif  // ifndef CONTROLLER_H