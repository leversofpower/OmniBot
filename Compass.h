//#ifndef __COMPASS_H__
//#define __COMPASS_H__

#include <Arduino.h>

class Compass {
public:
	Compass();
	void update();
	int headingDegrees;
	//float directionalHeading;
};

//#endif
