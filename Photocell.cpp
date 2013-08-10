// 
// 
// 

#include "Photocell.h"

void Photocell::init(){
	numberOfSensors = 4;
	updateInterval = 1000; //milliseconds
	updateIntervalCycleIndex = 0;
	photoCellPins[0] = 1;
	photoCellPins[1] = 2;
	photoCellPins[2] = 3;
	photoCellPins[3] = 4;
}
void Photocell::update(){
	if (millis() - lastUpdateTime > updateInterval) {
		lastUpdateTime = millis();
		for (int i = 0; i < numberOfSensors; i++)		{
			photoCellData[i].pin = i;
			photoCellData[i].time[updateIntervalCycleIndex] = millis();
			photoCellData[i].value[updateIntervalCycleIndex] = analogRead(photoCellPins[i]);
		}

		if (updateIntervalCycleIndex < 10){
			updateIntervalCycleIndex++;
		}
		else{
			updateIntervalCycleIndex = 0;
		}
	}
}
