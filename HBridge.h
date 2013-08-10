#ifndef __HBRIDGE_H__
#define __HBRIDGE_H__

#include <Arduino.h>

class HBridge {
  int _mtrAEnablePin;
  int _mtrBEnablePin;
  int _mtrA1Pin;
  int _mtrA2Pin;
  int _mtrB1Pin;
  int _mtrB2Pin;
  
  public:
    HBridge(int, int, int, int, int, int); //mtrAEnablePin, mtrBEnablePin, mtrA1Pin, mtrA2Pin, mtrB1Pin, mtrB2Pin);
    void init();
    void setBrake();

    void forward();
    void forward(int);
    void forward(int, int);

    void reverse();
    void reverse(int);
    void reverse(int, int);

    void rotateLeft();
    void rotateLeft(int);
    void rotateLeft(int, int);

    void rotateRight();
    void rotateRight(int);
    void rotateRight(int, int);
};

#endif
