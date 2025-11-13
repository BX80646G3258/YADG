#include "Display.h"
#include "src/modified/Font.h"  // modified font with up and down arrow
#include <ssd1306.h>

namespace Display {

void begin() {
  ssd1306_setFixedFont(font6x8);
  ssd1306_128x32_i2c_init();
}

void print(char const* const string, uint8_t row, uint8_t col) {
  ssd1306_printFixed(col * FONT_WIDTH, row * FONT_HEIGHT, string, STYLE_NORMAL);
}

void print_P(char const* const string, uint8_t row, uint8_t col) {
  uint8_t bufferSize = min(strlen_P(string), COLS) + 1;
  char* buffer = new char[bufferSize];
  strlcpy_P(buffer, string, bufferSize);
  print(buffer, row, col);
  delete[] buffer;
}

void print(int32_t i, uint8_t row, uint8_t col) {
  uint32_t compare = 1;
  uint32_t temp;
  uint8_t digits;

  if (i <= 0) {
    temp = -i;
    digits = 1;
  } else {
    temp = i;
    digits = 0;
  }

  while (compare <= temp) {
    compare *= 10;
    digits++;
  }
  char* buffer = new char[digits + 1];
  ltoa(i, buffer, 10);
  print(buffer, row, col);
  delete[] buffer;
}

void line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
  ssd1306_drawLine(x1, y1, x2, y2);
}

void setInvert(bool invert) {
  if (invert) {
    ssd1306_negativeMode();
  } else {
    ssd1306_positiveMode();
  }
}

void cls(bool invert) {
  setInvert(invert);
  ssd1306_clearScreen();
}
}  // namespace Display