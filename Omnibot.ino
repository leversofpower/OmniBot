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
#include "BluetoothController.h"
#include "RTC_DS1307.h"
#include "HBridge.h"
#include "Adafruit_NeoPixel.h"

//Sensors
Photocell PhotoCellSensorArray; 
Compass mainCompass;
AnalogVoltageDivider DriveTrainVoltMonitor;
RTC_DS1307 RTC;

//Controls
void (*pButtonMomentaryInterruptFunction)() = ButtonQuickPress;
ButtonMomentaryNoLed WhitePushButton(3, pButtonMomentaryInterruptFunction);

//BT JY-MCU v1.03 defaults to 9600.
SoftwareSerial bluetoothSoftwareSerial(10, 9); // RX, TX
BluetoothController AndroidController;

//Outputs
LEDPowerButton PowerButton;
LiquidCrystal_I2C LCDHardware(0x27, 20, 4);
LCD LCDdisplayManager(&LCDHardware, 20, 4);
MP3 MP3Hardware;
MP3player MP3manager;
HBridge Ardumotor(3,3,11,11,12,13);

//Neo Pixel
#define PIN 53

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(32, PIN);

uint8_t mode = 0, // Current animation effect
	offset = 0; // Position of spinny eyes
uint32_t color = 0x0088ff; //0xffae00; // Start red
uint32_t prevTime;

//Code to delete
int selectedTrack = 0;

void setup(){
	initializeHardware();

	//Plasma Switch
	pinMode(13, OUTPUT);

	LCDdisplayManager.addMessage("Initializing...");
	LCDdisplayManager.addMessage("January, 23rd 2014");
	LCDdisplayManager.addMessage("Last Soul Download");
	LCDdisplayManager.addMessage("Omnibot 2014");
	delay(3000); //Wait for MP3 player to startup
	MP3manager.playTrack(5);

	//Time Clock
	RTC.begin();
	DateTime now = RTC.now();
	DateTime compiled = DateTime(__DATE__, __TIME__);
	if (now.unixtime() < compiled.unixtime()) {
		Serial.println("RTC is older than compile time! Updating");
		// following line sets the RTC to the date & time this sketch was compiled
		RTC.adjust(DateTime(__DATE__, __TIME__));
	}

	// Neo Pixel Ring
	pixels.begin();
	pixels.setBrightness(01); // 1/3 brightness was 60
	prevTime = millis();


	/*
	Emic 2
	Command Set
	All commands are ASCII-based printable characters and are not case-sensitive (upper case and lower
	case will both work). Each command must be terminated with a CR or LF.
	Sx Convert text-to-speech: x = message (1023 characters maximum)
	Dx Play demonstration message: x = 0 (Speaking), 1 (Singing), 2 (Spanish)
	X Stop playback (while message is playing)
	Z Pause/un-pause playback (while message is playing)
	Nx Select voice: x = 0 to 8
	Vx Set audio volume (dB): x = -48 to 18
	Wx Set speaking rate (words/minute): x = 75 to 600
	Lx Select language: x = 0 (US English), 1 (Castilian Spanish), 2 (Latin Spanish)
	Px Select parser: x = 0 (DECtalk), 1 (Epson)
	R Revert to default text-to-speech settings
	C Print current text-to-speech settings
	I Print version information
	H Print list of available commands 
	*/
	Serial3.print('\n');             // Send a CR in case the system is already up
	while (Serial3.read() != ':');   // When the Emic 2 has initialized and is ready, it will send a single ':' character, so wait here until we receive it
	delay(10);                       // Short delay
	Serial3.flush();                 // Flush the receive buffer

	//Set Voice 0 to 8
	Serial3.print("N3\n");
	//Set Speaking rate
	Serial3.print("W170\n");
	//Set Volume -48 to 18
	Serial3.print("V13\n");
	// Speak some text
	Serial3.print('S');
	Serial3.print("Party robot two thousand");  // Send the desired string to convert to speech
	Serial3.print('\n');
	while (Serial3.read() != ':');   // Wait here until the Emic 2 responds with a ":" indicating it's ready to accept the next command

}
void loop(){
	LCDdisplayManager.update();
	mainCompass.update();
	PowerButton.update();
	BluetoothUpdate();
	DateTimeSerialPrint();

	/* Example Code
	//LCDdisplayManager.addMessage(mainCompass.headingDegrees);
	//Plasma Switch
	digitalWrite(13, HIGH);
	*/

	//Neo Pixel Rings
	uint8_t i;
	uint32_t t;

	switch(mode) {

	case 0: // Random sparks - just one LED on at a time!
			pixels.setBrightness(20); // 1/3 brightness was 60

		i = random(32);
		pixels.setPixelColor(i, color);
		pixels.show();
		delay(10);
		pixels.setPixelColor(i, 0);
		break;
	case 1: // Spinny wheels (8 LEDs on at a time)
			pixels.setBrightness(01); // 1/3 brightness was 60
		for(i=0; i<16; i++) {
			uint32_t c = 0;
			if(((offset + i) & 7) < 2) c = color; // 4 pixels on...
			pixels.setPixelColor( i, c); // First eye
			pixels.setPixelColor(31-i, c); // Second eye (flipped)
		}
		pixels.show();
		offset++;
		delay(20);
		break;
	}

	t = millis();
	if((t - prevTime) > 8000) { // Every 8 seconds...
		mode++; // Next mode
		if(mode > 1) { // End of modes?
			mode = 0; // Start modes over
			//color >>= 8; // Next color R->G->B
			//if(!color) color = 0xffae00; // Reset to red
		}
		for(i=0; i<32; i++) pixels.setPixelColor(i, 0);
		prevTime = t;
	}

}
void initializeHardware(){
	Serial.begin(9600);
	Serial1.begin(9600); //MP3 Player
	Serial2.begin(9600); //Bluetooth JY-MCU
	Serial3.begin(9600); //Emic 2

	PhotoCellSensorArray.init();
	DriveTrainVoltMonitor.init();
	PowerButton.init(4);

	LCDHardware.init();
	LCDHardware.backlight();
	LCDdisplayManager.setUpdateInterval(500);

}
void BluetoothUpdate(){
	//Bluetooth JY-MCU
	if (Serial2.available()) {
		while(Serial2.available()) {
			AndroidController.readSerial(Serial2.read());
		}
	}
}
void DateTimeSerialPrint(){
	DateTime now = RTC.now();
	Serial.print(now.year(), DEC);
	Serial.print('/');
	Serial.print(now.month(), DEC);
	Serial.print('/');
	Serial.print(now.day(), DEC);
	Serial.print(' ');
	Serial.print(now.hour(), DEC);
	Serial.print(':');
	Serial.print(now.minute(), DEC);
	Serial.print(':');
	Serial.print(now.second(), DEC);
	Serial.println();
}
void ButtonQuickPress()
{	
	if((WhitePushButton.isOff) && (digitalRead(WhitePushButton.interruptPin) == true)) {
		WhitePushButton.isOn = true;	
		WhitePushButton.lastPressed = millis();
		MP3manager.playTrack(selectedTrack);
		selectedTrack++;
		Serial3.print('S');
	Serial3.print(selectedTrack);  // Send the desired string to convert to speech
	Serial3.print('\n');
	}

	if((WhitePushButton.isOn) && (digitalRead(WhitePushButton.interruptPin) == false)) {
		WhitePushButton.isOn = false;
		WhitePushButton.isOff = true;
		WhitePushButton.lastDePressed = millis();
		WhitePushButton.lastTotalPressedTime = WhitePushButton.lastDePressed - WhitePushButton.lastPressed;
	}
}
