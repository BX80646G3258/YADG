#include "Profiler.h"
#include <Arduino.h>

Profiler::Profiler(bool enabled, uint16_t reportInterval)
  : enabled(enabled), reportInterval(reportInterval) {
  if (!enabled) {
    return;
  }

  entries = new ProfilerEntry[MAX_ENTRIES];
  lastEntry = nullptr;
  entryCount = 0;
  sampleCount = 0;
  lastSampleTimestamp = lastReportTimestamp = micros();
}

Profiler::~Profiler() {
  if (!enabled) {
    return;
  }

  delete[] entries;
}

void Profiler::sampleImpl(char const* const id) {
  uint32_t timestamp = micros();
  endSample(timestamp);

  for (uint8_t i = 0; i < entryCount; i++) {
    if (strcmp(id, (char const*)entries[i].id) == 0) {
      lastEntry = &entries[i];
      lastSampleTimestamp = micros();
      return;
    }
  }

  if (entryCount < MAX_ENTRIES) {
    strcpy(entries[entryCount].id, id);
    entryCount++;
  }
  lastSampleTimestamp = micros();
}

void Profiler::endImpl() {
  uint32_t timestamp = micros();
  endSample(timestamp);
  lastEntry = nullptr;
  sampleCount++;
  uint32_t elapsed = timestamp - lastReportTimestamp;
  if (sampleCount >= reportInterval) {
    for (uint8_t i = 0; i < entryCount; i++) {
      ProfilerEntry* entry = &entries[i];
      Serial.print((char*)entry->id);
      Serial.print('\t');
      // seem to lose 8.2us sampling
      Serial.print((float)entry->duration / sampleCount - 8.2);
      Serial.println();

      entry->duration = 0;
    }

    Serial.println(1000000.0 * sampleCount / elapsed);

    sampleCount = 0;
    lastReportTimestamp = micros();
  }
}


void Profiler::endSample(uint32_t timestamp) {
  if (lastEntry != nullptr) {
    uint32_t elapsed = timestamp - lastSampleTimestamp;
    lastEntry->duration += elapsed;
  }
}