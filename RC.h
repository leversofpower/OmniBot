#ifndef __RC_H__
#define __RC_H__
#include <Arduino.h>

class RC {
	struct RCRange {
		int bottom;
		int top;
	};
	RCRange _throttleRange;
	RCRange _steeringRange;

	int _throttlePin;
	int _steeringPin;

	int _currentThrottle;
	int _currentSteering;

	bool _invert;

public:
	RC();
	RC(int, int);

	int getThrottle();
	int getThrottle(int, int);
	void updateThrottle();
	void setThrottleRange(int, int);
	int getSteering();
	int getSteering(int, int);
	void updateSteering();
	void setSteeringRange(int, int);
	int normalize(int, int, int);
	void invertThrottle(bool);
};
#endif
