#include "AnalogVoltageDivider.h"
#include "Arduino.h"

void AnalogVoltageDivider::init(){
	updateInterval = 1000; //milliseconds
	updateIntervalCycleIndex = 0;
}
void AnalogVoltageDivider::update(){
	if (millis() - lastUpdateTime > updateInterval) {
		lastUpdateTime = millis();
		Data[0].time[updateIntervalCycleIndex] = millis();
		Data[0].value[updateIntervalCycleIndex] = analogRead(0);

		if (updateIntervalCycleIndex < 10){
			updateIntervalCycleIndex++;
		}
		else{
			updateIntervalCycleIndex = 0;
		}
	}
}