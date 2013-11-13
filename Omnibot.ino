#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MP3.h>
#include "LCD.h"
#include "Photocell.h"
#include "AnalogVoltageDivider.h"
#include "MP3player.h"
#include "Compass.h"
#include "LEDPowerButton.h"
#include "ButtonMomentaryNoLed.h"

LiquidCrystal_I2C LCDHardware(0x27, 20, 4);
LCD LCDdisplayManager(&LCDHardware, 20, 4);
MP3 MP3Hardware;
MP3player MP3manager;
Photocell PhotoCellSensorArray; 
AnalogVoltageDivider DriveTrainVoltMonitor;
Compass mainCompass;
LEDPowerButton PowerButton;

void (*pButtonMomentaryInterruptFunction)() = ButtonQuickPress;
ButtonMomentaryNoLed WhitePushButton(3, pButtonMomentaryInterruptFunction);

void initializeHardware(){
	Serial.begin(9600);
	Serial1.begin(9600); //MP3 Player

	PhotoCellSensorArray.init();
	DriveTrainVoltMonitor.init();
	PowerButton.init(4);

	LCDHardware.init();
	LCDHardware.backlight();
	LCDdisplayManager.setUpdateInterval(500);


}

void setup(){
	initializeHardware();

	//Button Code
	//pinMode(3, INPUT); 
	//attachInterrupt(1, buttonPress, CHANGE);

	//Plasma Switch
	pinMode(13, OUTPUT);

	LCDdisplayManager.addMessage("Initializing...");
	LCDdisplayManager.addMessage("October, 8th 2013");
	LCDdisplayManager.addMessage("Last Soul Download");
	LCDdisplayManager.addMessage("Omnibot 2014");
	delay(3000);
	MP3manager.playTrack(5);
}

void loop(){

	LCDdisplayManager.update();
	mainCompass.update();
	//LCDdisplayManager.addMessage(mainCompass.headingDegrees);
	PowerButton.update();

	//Plasma Switch
	digitalWrite(13, HIGH);
}

void ButtonQuickPress()
{	
	if((WhitePushButton.isOff) && (digitalRead(WhitePushButton.interruptPin) == true)) {
		WhitePushButton.isOn = true;	
		WhitePushButton.lastPressed = millis();
	}

	if((WhitePushButton.isOn) && (digitalRead(WhitePushButton.interruptPin) == false)) {
		WhitePushButton.isOn = false;
		WhitePushButton.isOff = true;
		WhitePushButton.lastDePressed = millis();
		WhitePushButton.lastTotalPressedTime = WhitePushButton.lastDePressed - WhitePushButton.lastPressed;
	}
	LCDdisplayManager.addMessage(mainCompass.headingDegrees);
	LCDdisplayManager.addMessage(WhitePushButton.lastPressed);
	LCDdisplayManager.addMessage(WhitePushButton.lastDePressed);
	LCDdisplayManager.addMessage(WhitePushButton.lastTotalPressedTime);
}
