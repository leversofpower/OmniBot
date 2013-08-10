#include "RC.h"

RC::RC(int throttlePin, int steeringPin) {
    pinMode(throttlePin, INPUT);
    pinMode(steeringPin, INPUT);

    _throttlePin = throttlePin;
    _steeringPin = steeringPin;
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

RCRange RC::getThrottleRange() {
    return _throttleRange;
}

int RC::getSteering() {
    getThottle(_steeringRange.bottom, _steeringRange.top);
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

RCRange RC::getSteeringRange() {
    return _steeringRange;
}

int RC::normalize(int value, int bottom, int top) {
    return constrain(map(value, bottom, top, -512, 512), -255, 255);
}
