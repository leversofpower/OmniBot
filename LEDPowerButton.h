// Untested

#ifndef _LEDPOWERBUTTON_h
#define _LEDPOWERBUTTON_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class LEDPowerButton
{
private:
	unsigned long lastPulseTime;

	int PWMdigitalPin;
	int PulseRate;
	int PulseSequence[27];
	int PulseSequencePosition;
	unsigned long LastPulseTime;
	void loadDefaultPulse();
public:
	void init(int LEDPWMpin);
	void setPulseRate(int rate);
	void update();
};

//extern LEDPowerButton LEDPOWERBUTTON;

#endif

