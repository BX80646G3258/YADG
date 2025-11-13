#ifndef EEPROMBUFFER_H
#define EEPROMBUFFER_H

#include <EEPROM.h>

class EEPROMBuffer {
public:
  enum Mode {
    WRITE,
    READ,
    DRY
  };

  template<typename T>
  void readWrite(T& data) {
    uint8_t* bytes = (uint8_t*)&data;
    for (uint16_t i = 0; i < sizeof(T); i++) {
      switch (mode) {
        case WRITE:
          EEPROM.update(index, bytes[i]);
          break;
        case READ:
          bytes[i] = EEPROM.read(index);
          break;
        case DRY:
          break;
      }
      index++;
    }
  }

  void readWriteString(char* buffer, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
      readWrite(buffer[i]);
    }
  }

  void seek(uint16_t index) {
    this->index = index;
  }

  uint16_t getPosition() {
    return index;
  }

  EEPROMBuffer(Mode mode)
    : mode(mode) {
    seek(0);
  }

private:
  Mode const mode;
  uint16_t index;
};
#endif  // ifndef EEPROMBUFFER_H