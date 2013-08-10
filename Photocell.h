// Photocell.h

#ifndef _PHOTOCELL_h
#define _PHOTOCELL_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class Photocell
{
private:
	struct photoArray
	{
		int pin;
		unsigned long time[11];
		int value[11];
	};
	int numberOfSensors;
	int updateInterval;
	int updateIntervalCycleIndex;
	unsigned long lastUpdateTime;
	int photoCellPins[4];
public:
	void init();
	void update();
	photoArray photoCellData[4];
};
#endif

