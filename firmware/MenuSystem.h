#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <stdint.h>
#include <EEPROM.h>

namespace MenuSystem {
static const uint16_t DEFAULT_PROFILE_ADDR = EEPROM.length() - 1;
void run();
void loadDefault();
}  // namespace MenuSystem

#endif  // ifndef MENUSYSTEM_H