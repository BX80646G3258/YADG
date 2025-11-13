#ifndef DISPLAY_H
#define DISPLAY_H
#include <stdint.h>

namespace Display {
static uint8_t const WIDTH = 128;
static uint8_t const HEIGHT = 32;

static uint8_t const FONT_WIDTH = 6;
static uint8_t const FONT_HEIGHT = 8;

static uint8_t const ROWS = HEIGHT / FONT_HEIGHT;
static uint8_t const COLS = WIDTH / FONT_WIDTH;

void begin();
void print(char const* const string, uint8_t row = 0, uint8_t col = 0);
void print_P(char const* const string, uint8_t row = 0, uint8_t col = 0);  // print PROGMEM c string to display
void print(int32_t i, uint8_t row = 0, uint8_t col = 0);
void line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void setInvert(bool invert);
void cls(bool invert = false);
}  // namespace Display

#endif  // ifndef DISPLAY_H