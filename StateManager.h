// StateManager.h

#ifndef _STATEMANAGER_h
#define _STATEMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class StateManager
{
 private:


 public:
	void init();
	/*Add State
	If statecollection first state is undefined then insert
	set the end time if the previous states
	shift states down one and add a new state
	set the start time of the new state
	*/
};

extern StateManager STATEMANAGER;

#endif

