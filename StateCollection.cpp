// 
// 
// 

#include "StateCollection.h"
#include "State.h"

void StateCollection::init()
{


}

State getState(int index){
	return STATECOLLECTION.states[index];
}

void addState(State newState){
	//int latestStateID = STATECOLLECTION.states[0].getID();
	//STATECOLLECTION.states[0].setStateEnd(millis());
	//for (int index = 0; index < 9; index++) {
	//	STATECOLLECTION.states[index + 1] = STATECOLLECTION.states[index];
	//}
	////After states are copied define the new state further
	//STATECOLLECTION.states[0] = newState;
	//STATECOLLECTION.states[0].setID(latestStateID++);
	//STATECOLLECTION.states[0].setStateStart(millis());
}

StateCollection STATECOLLECTION;

