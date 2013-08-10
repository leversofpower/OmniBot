#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MP3.h>
#include "LCD.h"
#include "Photocell.h"
#include "AnalogVoltageDivider.h"
#include "MP3player.h"
#include "RC.h"

LCD LCDdisplayManager;
LiquidCrystal_I2C LCDHardware(0x27, 20, 4);
MP3 MP3Hardware;
MP3player MP3manager;
RC RCcontroller(9,8);
Photocell PhotoCellSensorArray; 
AnalogVoltageDivider DriveTrainVoltMonitor;

void setup(){
	Serial.begin(9600);
	Serial1.begin(9600);

	initializeHardware();
	LCDdisplayManager.addMessage("Hello Human...");

	delay(5000);
	MP3manager.volume_high();
	MP3manager.single_play();

	RCcontroller.setThrottleRange(1059, 1972);
	RCcontroller.invertThrottle(1);
}

void loop(){
	PhotoCellSensorArray.update();
	DriveTrainVoltMonitor.update();
	LCDdisplayManager.update(LCDHardware);

	delay(1000);
	RCcontroller.updateThrottle();
	LCDdisplayManager.addMessage(String(RCcontroller.getThrottle()));
}

void initializeHardware(){
	PhotoCellSensorArray.init();
	DriveTrainVoltMonitor.init();
	LCDHardware.init();
	LCDHardware.backlight();
	LCDdisplayManager.setUpdateInterval(250);
}
