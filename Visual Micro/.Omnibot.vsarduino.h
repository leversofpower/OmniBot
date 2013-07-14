#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Mega 2560 or Mega ADK
#define __AVR_ATmega2560__
#define ARDUINO 104
#define __AVR__
#define F_CPU 16000000L
#define __cplusplus
#define __attribute__(x)
#define __inline__
#define __asm__(x)
#define __extension__
#define __ATTR_PURE__
#define __ATTR_CONST__
#define __inline__
#define __asm__ 
#define __volatile__
#define __builtin_va_list
#define __builtin_va_start
#define __builtin_va_end
#define __DOXYGEN__
#define prog_void
#define PGM_VOID_P int
#define NOINLINE __attribute__((noinline))

typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

void ArduinoMP3Shield_SendCMD(unsigned char *cmd_buf, unsigned int len);
void MP3StartUpPlay();
void MP3RCDetected();
void MP3RCLost();
int getIndexOfMaximumValue(int *array, int size);
void FaceTheLight();
void setCDSPins();
void setHBridgePins();
void setEyePins();
void automationStateSet(int stateType);
String automationStateVerboseFormat();
void backPowerButtonPulse();
void backPowerButtonPulseRateChange();
void eyesBlink();
void eyesOpen();
void eyesClose();
void CDSSensorRead();
void LCDdisplayOverride();
void LCDdisplay(String state);
void automationStateDisplay(int displayType);
void RCReadControlPWM();
bool RCcontrolIsMoveForwardRequested();
bool RCControlIsMoveBackwardRequested();
bool RCcontrolIsMoveRotateLeftRequested();
bool RCcontrolIsMoveRotateRightRequested();
void actionMoveForward();
void actionMoveBackward();
void actionMoveLeftRotate(int motorPWM);
void actionMoveRightRotate(int motorPWM);
void actionMoveSteer(int motorPWM, int state);
void actionMoveStop();
void actionStationary();
void RCAutomationStateSet();
void RCActivateState();
bool hallAlignWheels();
bool hallRotateRight();
void motorBatteryAnalogVoltageDividerRead();
void wheelHallSensorsRead();
//
//

#include "C:\Program Files\arduino-1.0.4\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Program Files\arduino-1.0.4\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\Omnibot.ino"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\OmniBot.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\PinDefinition.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\State.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\State.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\StateCollection.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\StateCollection.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\StateManager.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\StateManager.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\resource.h"
#endif
