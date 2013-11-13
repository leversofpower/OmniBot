#ifndef _VSARDUINO_H_
#define _VSARDUINO_H_
//Board = Arduino Mega 2560 or Mega ADK
#define __AVR_ATmega2560__
#define 
#define ARDUINO 104
#define ARDUINO_MAIN
#define __AVR__
#define F_CPU 16000000L
#define __cplusplus
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
#define __attribute__(x)
#define NOINLINE __attribute__((noinline))
#define prog_void
#define PGM_VOID_P int
            
typedef unsigned char byte;
extern "C" void __cxa_pure_virtual() {;}

void initializeHardware();
//
//
void ButtonQuickPress();

#include "C:\Program Files\arduino-1.0.4\hardware\arduino\variants\mega\pins_arduino.h" 
#include "C:\Program Files\arduino-1.0.4\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\Omnibot.ino"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\AnalogVoltageDivider.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\AnalogVoltageDivider.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\ButtonMomentaryNoLed.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\ButtonMomentaryNoLed.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\Clock.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\Clock.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\Compass.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\Compass.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\HBridge.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\HBridge.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\HMC5883L.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\HMC5883L.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\LCD.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\LCD.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\LEDPowerButton.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\LEDPowerButton.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\MP3player.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\MP3player.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\OmniBot.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\Photocell.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\Photocell.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\PinDefinition.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\RC.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\RC.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\RTC.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\RTC.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\RTC_DS1307.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\RTC_DS1307.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\State.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\State.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\StateCollection.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\StateCollection.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\StateManager.cpp"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\StateManager.h"
#include "C:\Users\gharris\Documents\GitHub\OmniBot\resource.h"
#endif
