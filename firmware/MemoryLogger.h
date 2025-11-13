#ifndef MEMORYLOGGER_H
#define MEMORYLOGGER_H

#include <stdint.h>

namespace MemoryLogger {
void begin();
uint16_t availableMemory();
}  // namespace MemoryLogger

#endif  // ifndef MEMORYLOGGER_H