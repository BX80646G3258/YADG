#ifndef BUTTONMATRIX_H
#define BUTTONMATRIX_H

#include "Debouncer.h"
#include "Profiler.h"
#include <Arduino.h>
#include <stdint.h>

template<uint8_t ROWS, uint8_t COLS> class ButtonMatrix {
public:
  static uint8_t const BUTTON_COUNT = ROWS * COLS;

  uint8_t const rowDelay;
  Debouncer debouncers[BUTTON_COUNT];

  ButtonMatrix(uint8_t const rowPins[ROWS], uint8_t const colPins[COLS], uint8_t rowDelay = 0);
  void begin();
  void update(bool buttonStates[BUTTON_COUNT]);
private:
  volatile uint8_t* rowRegisters[ROWS];
  volatile uint8_t* colRegisters[COLS];
  uint8_t rowMasks[ROWS];
  uint8_t colMasks[COLS];

  Profiler profiler;

  bool readCol(uint8_t c);
  void writeRow(uint8_t r, bool b);
};

template<uint8_t ROWS, uint8_t COLS> ButtonMatrix<ROWS, COLS>::ButtonMatrix(uint8_t const rowPins[ROWS], uint8_t const colPins[COLS], uint8_t rowDelay)
  : rowDelay(rowDelay), profiler(false) {
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);

    colRegisters[i] = portInputRegister(digitalPinToPort(colPins[i]));
    colMasks[i] = digitalPinToBitMask(colPins[i]);
  }

  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);

    uint8_t port = digitalPinToPort(rowPins[i]);
    if (port == NOT_A_PIN) {
      rowRegisters[i] = nullptr;
    } else {
      rowRegisters[i] = portOutputRegister(port);
    }
    rowMasks[i] = digitalPinToBitMask(rowPins[i]);
  }
}

template<uint8_t ROWS, uint8_t COLS> void ButtonMatrix<ROWS, COLS>::begin() {
  writeRow(0, false);
  for (uint8_t i = 0; i < ROWS; i++) {
    writeRow(i, true);
  }
}

template<uint8_t ROWS, uint8_t COLS> void ButtonMatrix<ROWS, COLS>::update(bool buttonStates[ROWS * COLS]) {
  uint32_t timestamp = micros();  // micros() is expensive
  for (uint8_t r = 0; r < ROWS; r++) {
    profiler.sample("dlay");
    bool rowBuffer[COLS];
    delayMicroseconds(rowDelay);

    profiler.sample("read");
    for (uint8_t c = 0; c < COLS; c++) {
      rowBuffer[c] = readCol(c);
    }

    profiler.sample("writ");
    writeRow(r, true);
    if (r + 1 < ROWS) {
      writeRow(r + 1, false);
    } else {
      writeRow(0, false);
    }

    profiler.sample("updt");
    for (uint8_t c = 0; c < COLS; c++) {
      uint8_t i = r * COLS + c;
      buttonStates[i] = debouncers[i].Update(!rowBuffer[c], timestamp);
    }
    profiler.end();
  }
}

template<uint8_t ROWS, uint8_t COLS> inline bool ButtonMatrix<ROWS, COLS>::readCol(uint8_t c) {
  return (*colRegisters[c] & colMasks[c]) ? true : false;
}

template<uint8_t ROWS, uint8_t COLS> inline void ButtonMatrix<ROWS, COLS>::writeRow(uint8_t r, bool b) {
  uint8_t bit = rowMasks[r];
  volatile uint8_t* out = rowRegisters[r];

  // if (out == nullptr) {
  //   return;
  // }

  uint8_t oldSREG = SREG;
  cli();

  if (b) {
    *out |= bit;
  } else {
    *out &= ~bit;
  }

  SREG = oldSREG;
}

#endif // ifndef BUTTONMATRIX_H