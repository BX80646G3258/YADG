#include "src/modified/USB_XInput_Descriptors.cpp"  // disgusting hack to modify USB_ConfigDescriptor

#include "Controller.h"
#include "ButtonMap.h"
#include "MenuSystem.h"
#include "UIStrings.h"
#include "Display.h"
#include "MemoryLogger.h"

#include <XInput.h>

using namespace ButtonMap;

const bool BENCHMARK = false;

uint32_t timestamp = micros();
uint16_t count = 0;

void showStartupLogo() {
  Display::cls();
  Display::print_P(UIStrings::STARTUP, 1, 2);
  Display::print_P(UIStrings::STARTUP2, 2, 4);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  // MemoryLogger::begin();

  controller.begin();
  MenuSystem::loadDefault();

  XInput.setAutoSend(false);
  XInput.begin();

  Display::begin();
  showStartupLogo();

  Display::cls();
}

void loop() {
  controller.update();

  if (controller.state.buttons[HW_CONF]) {
    XInput.releaseAll();
    XInput.send();
    MenuSystem::run();
    XInput.releaseAll();
    return;
  } else {
    // |= operation ensures that even when two hardware buttons are mapped to one software button, they both work
    bool buttons[sizeof(controller.state.buttons)] = { false };
    for (uint8_t i = 0; i < sizeof(buttons); i++) {
      buttons[controller.mapIndex(i)] |= controller.state.buttons[i];
    }

    for (uint8_t i = 0; i < sizeof(buttons) - 1; i++) {  // skip config button
      XInput.setButton(i, buttons[i]);
    }

    XInput.setJoystick(JOY_LEFT, controller.state.joystickLX, controller.state.joystickLY);
    XInput.setJoystick(JOY_RIGHT, controller.state.joystickRX, controller.state.joystickRY);
    XInput.setTrigger(TRIGGER_LEFT, controller.state.triggerL >> 8);
    XInput.setTrigger(TRIGGER_RIGHT, controller.state.triggerR >> 8);

    if (!BENCHMARK) {
      XInput.send();
    }
  }

  if (BENCHMARK) {
    count++;
    if (count > 1000) {
      // print the frequency of the main loop
      Serial.println((float)count * 1000000 / (micros() - timestamp));
      count = 0;
      timestamp = micros();
    }
  }
}
