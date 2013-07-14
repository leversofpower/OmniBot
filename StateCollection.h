// StateCollection.h
#include "State.h"

#ifndef _STATECOLLECTION_h
#define _STATECOLLECTION_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class StateCollection
{
	//TODO: Set StateMaxCount = 10;
private:

public:
	void init();
	State states[10];
	State getState(int index);
	void addState(State);

	enum Definition {
		stateUndefined = 0,
		NoRCSignal = 5,
		Stationary = 10,
		MoveForward = 20,
		MoveForwardSteerLeft = 22,
		MoveForwardSteerRight = 24,
		MoveBackward = 30,
		MoveBackwardSteerLeft = 32,
		MoveBackwardSteerRight = 34,
		MoveRotateLeft = 40,
		MoveRotateRight = 50,
	} stateType;
};

extern StateCollection STATECOLLECTION;
#endif

