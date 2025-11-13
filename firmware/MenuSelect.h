#ifndef MENUSELECT_H
#define MENUSELECT_H

#include <stdint.h>

namespace MenuSelect {
void select(void (*const entries[])(), char const* const names[], const uint8_t count);
bool selectIndex(char const* const names[], const uint8_t count, uint8_t& selected, uint8_t& scroll);
}  // namespace MenuSelect

#endif  // ifndef MENUSELECT_H