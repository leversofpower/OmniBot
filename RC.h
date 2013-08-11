#ifndef __RC_H__
#define __RC_H__
#include <Arduino.h>

class RC {
	struct RCRange {
		int bottom;
		int top;
		int deadBandTop;
		int deadBandBottom;
	};
	RCRange _throttleRange;
	RCRange _steeringRange;

	int _throttlePin;
	int _steeringPin;

	int _currentThrottle;
	int _currentSteering;

	bool _invertThrottle;
	int DeadBandTop_get(int maximumPWM, int minimumPWM);
	int DeadBandBottom_get(int maximumPWM, int minimumPWM);

public:
	RC(int throttlePin, int steeringPin, int throttleTop, int throttleBottom, int steeringTop, int steeringBottom, bool invertedThrottle);
	int getThrottle();
	int getSteering();
	int normalizeThrottle(int);
	int normalizeSteering(int);
};
#endif
