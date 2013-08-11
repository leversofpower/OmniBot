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
RC RCcontroller(9, 8, 1972, 1059, 1980, 1048, 1);
Photocell PhotoCellSensorArray; 
AnalogVoltageDivider DriveTrainVoltMonitor;

void initializeHardware(){
	Serial.begin(9600);
	Serial1.begin(9600);
	PhotoCellSensorArray.init();
	DriveTrainVoltMonitor.init();
	LCDHardware.init();
	LCDHardware.backlight();
	LCDdisplayManager.setUpdateInterval(250);
}

void setup(){
	initializeHardware();
	LCDdisplayManager.addMessage("Omnibot 2013");
}

void loop(){
	PhotoCellSensorArray.update();
	DriveTrainVoltMonitor.update();
	LCDdisplayManager.update(LCDHardware);
	delay(500);
	LCDdisplayManager.addMessage(String(RCcontroller.getThrottle()) + " / " + String(RCcontroller.getSteering()));
}


