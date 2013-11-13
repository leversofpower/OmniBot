#include "LEDPowerButton.h"

void LEDPowerButton::init(int LEDPWMpin)
{
	PWMdigitalPin = LEDPWMpin;
	PulseRate = 90; //time between beats
	PulseSequencePosition = 0;
	LastPulseTime = 0;
	loadDefaultPulse();
}
void LEDPowerButton::loadDefaultPulse(){
	int defaultPulseSequence[27] = {0,1,2,5,10,255,20,30,40,50,60,70,80,90,100,255,100,90,80,70,60,50,40,30,20,10,0};
	// copy default sequence into class array
	for (int i = 0; i < 27; i++) {
		PulseSequence[i] = defaultPulseSequence[i];
	}
}
void LEDPowerButton::update(){
	if (millis() - lastPulseTime > PulseRate) {
		if (PulseSequencePosition < 24) {
			lastPulseTime = millis();
			PulseSequencePosition = ++PulseSequencePosition;
		} else { 
			PulseSequencePosition = 0;
		}
		analogWrite(PWMdigitalPin, PulseSequence[PulseSequencePosition]);
	}
}

void LEDPowerButton::setPulseRate(int rate){
	PulseRate = rate;
}
