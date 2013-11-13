#ifndef __LCD_H__
#define __LCD_H__

#define LCD_BUFFER_LEN 20

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class LCD {
  LiquidCrystal_I2C *_hardware;
  unsigned int _characters;
  unsigned int _lines;
  
  unsigned long _lastUpdateTime;
  unsigned long _updateInterval;
  
  char *_queue[LCD_BUFFER_LEN];
public:
  LCD();
  LCD(LiquidCrystal_I2C *, unsigned int, unsigned int);
  
  void update();
  void addMessage(long);
  void addMessage(unsigned long);
  void addMessage(int);
  void addMessage(unsigned int);
  void addMessage(char *message);
  void addMessage(String *message);
  void setUpdateInterval(unsigned long);
};

#endif
