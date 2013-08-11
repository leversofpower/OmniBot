#include "RC.h"
RC::RC(int throttlePin, int steeringPin, int throttleTop, int throttleBottom, int steeringTop, int steeringBottom, bool invertThrottle) {
	pinMode(throttlePin, INPUT);
	pinMode(steeringPin, INPUT);

	_throttlePin = throttlePin;
	_steeringPin = steeringPin;

	_throttleRange.top = throttleTop;
	_throttleRange.bottom = throttleBottom;
	_throttleRange.deadBandTop = DeadBandTop_get(throttleTop, throttleBottom);
	_throttleRange.deadBandBottom = DeadBandBottom_get(throttleTop, throttleBottom);

	_steeringRange.top = steeringTop;
	_steeringRange.bottom = steeringBottom;
	_steeringRange.deadBandTop = DeadBandTop_get(steeringTop, steeringBottom);
	_steeringRange.deadBandBottom = DeadBandBottom_get(steeringTop, steeringBottom);

	_invertThrottle = invertThrottle;
}

int RC::getThrottle() {
	return normalizeThrottle(pulseIn(_throttlePin, HIGH, 20000));
}

int RC::normalizeThrottle(int value) {
	if ((value > 0) && 
		(value > _throttleRange.deadBandTop || value < _throttleRange.deadBandBottom)) {
			int PWMvalue = map(value, _throttleRange.bottom, _throttleRange.top, -255, 255);
			if (_invertThrottle) {
				PWMvalue *= -1;
			}
			return PWMvalue;
	}
	else {
		return 0;
	}
}
int RC::getSteering() {
	return normalizeSteering(pulseIn(_steeringPin, HIGH, 20000));
}

int RC::normalizeSteering(int value) {
	if ((value > 0) && 
		(value > _steeringRange.deadBandTop || value < _steeringRange.deadBandBottom)) {
			int PWMvalue = map(value, _steeringRange.bottom, _steeringRange.top, -255, 255);
			return PWMvalue;
	}
	else {
		return 0;
	}
}
int RC::DeadBandTop_get(int maximumPWM, int minimumPWM){
	int PWMSpread = maximumPWM - minimumPWM;
	int PWMmiddle = (PWMSpread / 2) + minimumPWM;
	return PWMmiddle + 20; //TODO:Buffer const
}
int RC::DeadBandBottom_get(int maximumPWM, int minimumPWM){
	int PWMSpread = maximumPWM - minimumPWM;
	int PWMmiddle = (PWMSpread / 2) + minimumPWM;
	return PWMmiddle - 20; //TODO:Buffer const
}