//
//  OmniBot.h
//  OmniBot
//
//  Created by Brandon Peters on 7/14/13.
//  Copyright (c) 2013 Gabe Harris. All rights reserved.
//

#ifndef OmniBot_OmniBot_h
#define OmniBot_OmniBot_h

enum state {
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
};

struct RCcontrollerPWM
{
	int current;
	int maximum;
	int resting;
	int minimum;
	int tolerence;
};

struct photoCell
{
	int frount;
	int back;
	int left;
	int right;
	int top;
};

void ArduinoMP3Shield_SendCMD(unsigned char, unsigned int);
void MP3StartUpPlay();
void MP3RCDetected();
void MP3RCLost();
int getIndexOfMaximumValue(int *, int);
void FaceTheLight();
void setCDSPins();
void setHBridgePins();
void setEyePins();
void automationStateSet(int);
String automationStateVerboseFormat();
void backPowerButtonPulse();
void backPowerButtonPulseRateChange();
void eyesBlink();
void eyesOpen();
void eyesClose();
void CDSSensorRead();
void LCDdisplayOverride();
void LCDdisplay(String);
void automationStateDisplay(int);

// RC
void RCAutomationStateSet();
void RCActivateState();
void RCReadControlPWM();
bool RCcontrolIsMoveForwardRequested();
bool RCControlIsMoveBackwardRequested();
bool RCcontrolIsMoveRotateLeftRequested();
bool RCcontrolIsMoveRotateRightRequested();

// Move Actions
void actionMoveForward();
void actionMoveBackward();
void actionMoveLeftRotate(int);
void actionMoveRightRotate(int);
void actionMoveSteer(int, int);
void actionMoveStop();
void actionStationary();

// Hall Sensor
bool hallAlignWheels();
bool hallRotateRight();

void motorBatteryAnalogVoltageDividerRead();
void wheelHallSensorsRead();
#endif
