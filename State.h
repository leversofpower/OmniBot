// State.h

#ifndef _STATE_h
#define _STATE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class State
{
private:
	int ID;
	int definitionID; //The enumeration goes here

	unsigned long startMillis;
	unsigned long endMillis;
	unsigned long durationMillis;

public:
	State();
	void setID(int);
	int getID() const;
	void setDefinitionID(int);
	int getDefinitionID() const;
	unsigned long getDuration() const;
	void setStateStart(unsigned long);
	void setStateEnd(unsigned long);
};

extern State STATE;

#endif

