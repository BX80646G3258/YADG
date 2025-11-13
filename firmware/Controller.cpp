#include "Controller.h"
#include "UIStrings.h"
#include "EEPROMBuffer.h"
#include <EEPROM.h>

using namespace ButtonMap;

Controller::Controller()
  : buttonMatrix(MATRIX_ROW_PINS, MATRIX_COL_PINS),
    triggerL(TL, true),
    triggerR(TR, true),
    joystickL(JLX, JLY, true, false),
    joystickR(JRX, JRY, false, true),
    profiler(false) {
}

void Controller::begin() {
  // fast ADC
  ADCSRA = (ADCSRA & 0b11111000) | 0b100;

  buttonMatrix.begin();
  resetAll();
}

void Controller::update() {
  profiler.sample("butt");
  memcpy(state.lastButtons, state.buttons, sizeof(state.lastButtons));
  buttonMatrix.update(state.buttons);

  profiler.sample("trig");
  triggerL.update(state.triggerL);
  triggerR.update(state.triggerR);

  profiler.sample("joy");
  joystickL.update(state.joystickLX, state.joystickLY);
  joystickR.update(state.joystickRX, state.joystickRY);

  profiler.end();
}

uint8_t Controller::mapIndex(uint8_t i) {
  return buttonMap[i];
}

void Controller::resetAll() {
  triggerL.resetAll();
  triggerR.resetAll();
  joystickL.resetAll();
  joystickR.resetAll();
  for (uint8_t i = 0; i < buttonMatrix.BUTTON_COUNT; i++) {
    buttonMatrix.debouncers[i].resetAll();
  }
  memcpy_P(buttonMap, DEFAULT_BUTTON_MAP, sizeof(buttonMap));
}

void Controller::factoryReset() {
  for (uint16_t i = 0; i < EEPROM.length(); i++) {
    EEPROM.update(i, 0xFF);
  }
}

uint16_t Controller::saveLoad(uint8_t index, EEPROMBuffer::Mode mode) {
  if (index > EEPROM.length() / PROFILE_SIZE) {
    return 0;
  }

  EEPROMBuffer eeprom(mode);
  eeprom.seek(index * PROFILE_SIZE);

  char const magic[] = "YADSGP";
  if (mode == EEPROMBuffer::READ) {
    char buffer[sizeof(magic)];
    eeprom.readWriteString(buffer, sizeof(magic));
    if (strcmp(magic, buffer) != 0) {
      return 0;
    }
  } else {
    eeprom.readWriteString((char*)magic, sizeof(magic));
  }

  triggerL.saveLoad(eeprom);
  triggerR.saveLoad(eeprom);
  joystickL.saveLoad(eeprom);
  joystickR.saveLoad(eeprom);

  for (uint8_t i = 0; i < buttonMatrix.BUTTON_COUNT; i++) {
    buttonMatrix.debouncers[i].saveLoad(eeprom);
  }

  for (uint8_t i = 0; i < sizeof(buttonMap); i++) {
    eeprom.readWrite(buttonMap[i]);
  }

  return eeprom.getPosition();
}

Controller controller;