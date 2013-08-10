// Needs tested
#include <LiquidCrystal_I2C.h>
#ifndef _LCD_h
#define _LCD_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class LCD
{
private:
	unsigned long lastUpdateTime;
	unsigned long updateInterval;
	struct displayMessage
	{
		String message;
	};
	displayMessage displayQue[20];

public:
	void init();
	void update(LiquidCrystal_I2C);
	void addMessage(String message);
	void setUpdateInterval(unsigned long);
};

#endif

