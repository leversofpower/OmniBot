#include "ButtonMomentaryNoLed.h"
#include "Arduino.h"

ButtonMomentaryNoLed::ButtonMomentaryNoLed(int pin,  void (*pInterruptFuncton)()){
	interruptPin = pin;
	pinMode(pin, INPUT); 
	attachInterrupt(1, pInterruptFuncton, CHANGE);
	isOn = false;
	isOff = true;
	lastPressed = 0;
	lastDePressed = 0;
	lastTotalPressedTime = 0;
}
