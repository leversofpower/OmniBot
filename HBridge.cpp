#include "HBridge.h"

HBridge::HBridge(int mtrAEnablePin, int mtrBEnablePin, int mtrA1Pin, int mtrA2Pin, int mtrB1Pin, int mtrB2Pin) {
  _mtrAEnablePin = mtrAEnablePin;
  _mtrBEnablePin = mtrBEnablePin;
  _mtrA1Pin = mtrA1Pin;
  _mtrA2Pin = mtrA2Pin;
  _mtrB1Pin = mtrB1Pin;
  _mtrB2Pin = mtrB2Pin;
}

void HBridge::init() {
  pinMode(_mtrAEnablePin, OUTPUT);
  pinMode(_mtrBEnablePin, OUTPUT);
  
  pinMode(_mtrA1Pin, OUTPUT);
  pinMode(_mtrA2Pin, OUTPUT);
  pinMode(_mtrB1Pin, OUTPUT);
  pinMode(_mtrB2Pin, OUTPUT);
}

void HBridge::setBrake() {
  digitalWrite(_mtrAEnablePin, LOW);
  digitalWrite(_mtrBEnablePin, LOW);
}

void HBridge::forward() {
    forward(HIGH);
}

void HBridge::forward(int pwm) {
    forward(pwm, pwm);
}

void HBridge::forward(int pwmA, int pwmB) {
  analogWrite(_mtrAEnablePin, pwmA);
  analogWrite(_mtrBEnablePin, pwmB);
  
  digitalWrite(_mtrA1Pin, HIGH);
  digitalWrite(_mtrA2Pin, LOW);
  
  digitalWrite(_mtrB1Pin, HIGH);
  digitalWrite(_mtrB2Pin, LOW);
}

void HBridge::reverse() {
    reverse(HIGH);
}

void HBridge::reverse(int pwm) {
    reverse(pwm, pwm);
}

void HBridge::reverse(int pwmA, int pwmB) {
  analogWrite(_mtrAEnablePin, pwmA);
  analogWrite(_mtrBEnablePin, pwmB);
  
  digitalWrite(_mtrA1Pin, LOW);
  digitalWrite(_mtrA2Pin, HIGH);
  
  digitalWrite(_mtrB1Pin, LOW);
  digitalWrite(_mtrB2Pin, HIGH);
}

void HBridge::rotateLeft() {
    rotateLeft(HIGH);
}

void HBridge::rotateLeft(int pwm) {
    rotateLeft(pwm, pwm);
}

void HBridge::rotateLeft(int pwmA, int pwmB) {
  analogWrite(_mtrAEnablePin, pwmA);
  analogWrite(_mtrBEnablePin, pwmB);
  
  digitalWrite(_mtrA1Pin, LOW);
  digitalWrite(_mtrA2Pin, HIGH);
  
  digitalWrite(_mtrB1Pin, HIGH);
  digitalWrite(_mtrB2Pin, LOW);
}

void HBridge::rotateRight() {
    rotateRight(HIGH);
}

void HBridge::rotateRight(int pwm) {
    rotateRight(pwm, pwm);
}

void HBridge::rotateRight(int pwmA, int pwmB) {
  analogWrite(_mtrAEnablePin, pwmA);
  analogWrite(_mtrBEnablePin, pwmB);
  
  digitalWrite(_mtrA1Pin, HIGH);
  digitalWrite(_mtrA2Pin, LOW);
  
  digitalWrite(_mtrB1Pin, LOW);
  digitalWrite(_mtrB2Pin, HIGH);
}
