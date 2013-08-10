#ifndef __RC_H__
#define __RC_H__

struct RCRange {
    int bottom;
    int top;
};

class RC {
    RCRange _throttleRange;
    RCRange _steeringRange;

    int _throttlePin;
    int _steeringPin;

    int _currentThrottle;
    int _currentSteering;
public:
    RC(int, int);

    int getThrottle();
    int getThrottle(int, int);
    void updateThrottle();

    void setThrottleRange(int, int);
    RCRange getThrottleRange();

    int getSteering();
    int getSteering(int, int);
    void updateSteering();

    void setSteeringRange(int, int);
    RCRange getSteeringRange();

    int normalize(int, int, int);
}
