#include <LiquidCrystal_I2C.h>
#include "LCD.h"

LCD::LCD() {}

LCD::LCD(LiquidCrystal_I2C *hardware, unsigned int characters, unsigned int lines) {
  _hardware = hardware;
  _characters = characters;
  _lines = lines;
  
  for (int i = 0; i < LCD_BUFFER_LEN; i++) {
    _queue[i] = new char[characters + 1];
  }
}

void LCD::update() {
  if (millis() - _lastUpdateTime > _updateInterval) {
    _hardware->clear();

    for (int i = 0; i < _lines; i++) {
      _hardware->setCursor(0, i);
      _hardware->print(_queue[i]);
    }

    _lastUpdateTime = millis();
  }
}

void LCD::addMessage(long message) {
  char buffer[_characters];
  snprintf(buffer, _characters, "%l", message);
  
  addMessage(buffer);
}

void LCD::addMessage(unsigned long message) {
  char buffer[_characters];
  snprintf(buffer, _characters, "%lu", message);
  
  addMessage(buffer);
}

void LCD::addMessage(int message) {
  char buffer[_characters];
  snprintf(buffer, _characters, "%d", message);
  
  addMessage(buffer);
}

void LCD::addMessage(unsigned int message) {
  char buffer[_characters];
  snprintf(buffer, _characters, "%du", message);
  
  addMessage(buffer);
}

void LCD::addMessage(char *message) {
  for (int i = LCD_BUFFER_LEN - 2; i >= 0; i--) {
    strncpy(_queue[i + 1], _queue[i], _characters);
  }
  
  strncpy(_queue[0], message, _characters);
}

void LCD::setUpdateInterval(unsigned long interval) {
  _updateInterval = interval;
}
