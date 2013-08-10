#include "RC.h"
RC::RC(){
}
RC::RC(int throttlePin, int steeringPin) {
	pinMode(throttlePin, INPUT);
	pinMode(steeringPin, INPUT);

	_throttlePin = throttlePin;
	_steeringPin = steeringPin;

	_invert = false;
}

int RC::getThrottle() {
	getThrottle(_throttleRange.bottom, _throttleRange.top);
}

int RC::getThrottle(int bottom, int top) {
	setThrottleRange(bottom, top);
	return normalize(_currentThrottle, bottom, top);
}

void RC::updateThrottle() {
	_currentThrottle = pulseIn(_throttlePin, HIGH, 20000); // TODO: Configure the 25000 somewhere 
}

void RC::setThrottleRange(int bottom, int top) {
	_throttleRange.bottom = bottom;
	_throttleRange.top = top;
}

int RC::getSteering() {
	getThrottle(_steeringRange.bottom, _steeringRange.top);
}

int RC::getSteering(int bottom, int top) {
	setSteeringRange(bottom, top);
	return normalize(_currentSteering, bottom, top);
}

void RC::updateSteering() {
	_currentSteering = pulseIn(_steeringPin, HIGH, 20000); // TODO: Configure the 25000 somewhere 
}

void RC::setSteeringRange(int bottom, int top) {
	_steeringRange.bottom = bottom;
	_steeringRange.top = top;
}

int RC::normalize(int value, int bottom, int top) {
	int PWMvalue = map(value, bottom, top, -255, 255);
	if (_invert) {
		PWMvalue *= -1;
	}

	return PWMvalue;
}

void RC::invertThrottle(bool invert) {
	_invert = invert;
};
