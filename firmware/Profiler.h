#ifndef PROFILER_H
#define PROFILER_H

#include <stdint.h>

class Profiler {
public:
  static uint8_t const MAX_ENTRIES = 16;
  static uint8_t const ID_LENGTH = 4;

  struct ProfilerEntry {
    char id[ID_LENGTH + 1];
    uint32_t duration = 0;
  };

  bool const enabled;
  uint16_t const reportInterval;

  ProfilerEntry* entries;
  ProfilerEntry* lastEntry;
  uint8_t entryCount;
  uint16_t sampleCount;
  uint32_t lastSampleTimestamp;
  uint32_t lastReportTimestamp;

  Profiler(bool enabled, uint16_t reportInterval = 1000);
  ~Profiler();

  // these wrappers allow the function calls to be optimized away when the profiler is disabled
  __attribute__((always_inline)) inline void sample(const char* const id) {
    if (enabled) {
      sampleImpl(id);
    }
  }

  __attribute__((always_inline)) inline void end() {
    if (enabled) {
      endImpl();
    }
  }

private:
  void sampleImpl(const char* const id);
  void endImpl();
  void endSample(uint32_t timestamp);
  void report();
};

#endif  // ifndef PROFILER_H