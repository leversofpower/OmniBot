#include "LCD.h"
#include <LiquidCrystal_I2C.h>

void LCD::init()
{
}
void LCD::update(LiquidCrystal_I2C LCDHardware){
	if (millis() - lastUpdateTime > updateInterval) {
		//TODO: Add length of display time check
		LCDHardware.clear();
		LCDHardware.setCursor(0,0);
		LCDHardware.print(displayQue[0].message);
		LCDHardware.setCursor(0,1);
		LCDHardware.print(displayQue[1].message);
		LCDHardware.setCursor(0,2);
		LCDHardware.print(displayQue[2].message);
		LCDHardware.setCursor(0,3);
		LCDHardware.print(displayQue[3].message);
		lastUpdateTime = millis();
	}
}
void LCD::addMessage(String message){
	for (int i = 18; i >= 0; i--) //TODO: 18 replace with constant
	{
		displayQue[i + 1].message = displayQue[i].message;
	}
	displayQue[0].message = String(message);
}
void LCD::setUpdateInterval(unsigned long updateIntervalMillis){
	updateInterval = updateIntervalMillis;
}