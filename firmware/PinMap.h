#ifndef PINMAP_H
#define PINMAP_H

#include <Arduino.h>
#include <stdint.h>

static uint8_t const MATRIX_ROW_PINS[] = { 8, 9, 10, 11 };
static uint8_t const MATRIX_ROW_COUNT = sizeof(MATRIX_ROW_PINS) / sizeof(MATRIX_ROW_PINS[0]);

static uint8_t const MATRIX_COL_PINS[] = { 4, 5, 6, 7 };
static uint8_t const MATRIX_COL_COUNT = sizeof(MATRIX_COL_PINS) / sizeof(MATRIX_COL_PINS[0]);

static uint8_t const TL = A0;
static uint8_t const TR = A5;

static uint8_t const JLX = A1;
static uint8_t const JLY = A2;

static uint8_t const JRX = A3;
static uint8_t const JRY = A4;

#endif // ifndef PINMAP_H