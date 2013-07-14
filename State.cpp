// 
// 
// 

#include "State.h"

State::State(){
	ID = 0;
}
void State::setID(int newID)
{
	ID = newID;
}
int State::getID() const
{
	return ID;
}
void State::setDefinitionID(int stateDefinitionID){
	definitionID = stateDefinitionID;
}
int State::getDefinitionID() const{
	return definitionID;
}
unsigned long State::getDuration() const
{
	return durationMillis;
} 
void State::setStateStart(unsigned long startTime){
	startMillis = startTime;
}
void State::setStateEnd(unsigned long endTime){
	endMillis = endTime;
}

State STATE();