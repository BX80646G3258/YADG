#include "MemoryLogger.h"
#include <Arduino.h>

namespace MemoryLogger {
uint16_t availableMemory() {
  uint16_t size = 2560;
  uint8_t* buf;
  while ((buf = (uint8_t*)malloc(--size)) == NULL) {};
  free(buf);
  return size;
}

void begin() {
  noInterrupts();
  TCCR1A = 0;                   // no gpio
  TCCR1B = 0b00001101;          // CTC to OCR1A, 1024 prescaler
  OCR1A = 16000000 / 1024 - 1;  // count to 1s
  TCNT1 = 0;                    // clear counter
  TIMSK1 |= 1 << OCIE1A;        // enable interrupt
  interrupts();
}

ISR(TIMER1_COMPA_vect) {
  Serial.println(availableMemory());
}

}  // namespace MemoryLogger