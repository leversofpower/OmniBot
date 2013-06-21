/*
TODO:
Fix Right rotation PWM clipping on the extreme right PWM scale
rotate one pass of hall sensor to check rotation angle
Break Blinking into two seperate timers. One for lid down and one for lid up.
Create array for the states. Add time spent in state for each state. Add last activate time of day.
State Based LCD Display:
Create LCD Message que
*/

/* Includes */
#include <Timer.h>
#include <Event.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//http://www.dfrobot.com/index.php?route=product/product&filter_name=DFR0063&product_id=135#.Ub9pOZzm8cl

/* Pin Assignments */
//RC Controller
const int RCcontrolY = 9;
const int RCcontrolX = 8;
const int motorLeftForward = 3;
const int motorLeftBackward = 4;
//H-Bridge
const int motorLeftPWM = 5; //I think this is wired backwards...
const int motorRightForward = 7;
const int motorRightBackward = 6; 
const int motorRightPWM = 2; //I think this is wired backwards...
//Hall Sensors
int wheelHallSensorLeft = 0;
int wheelHallSensorRight = 0;

/****************Need Deffinition*****************/
//Current Sensor
const int AnalogVoltageDividerPin = 0;
//Hall Sensors DEFINE
const int wheelLefttHallSensorPin = 0;
const int wheelRightHallSensorPin = 0;
//Vibration Sensor DEFINE
const int vibrationSensor = 0;
/**************************************************/

//CDS Sensors
const int CDSSensor1Frount = 1;
const int CDSSensor2Back = 2;
const int CDSSensor3Left = 3;
const int CDSSensor4Right = 4;
const int CDSSensor5Top = 5;

// Right Eye
const int ledPinEyeRight0 = 32;
const int ledPinEyeRight1 = 38; 
const int ledPinEyeRight2 = 31;
const int ledPinEyeRight3 = 29;
const int ledPinEyeRight4 = 27;
const int ledPinEyeRight5 = 30;
const int ledPinEyeRight6 = 28;
const int ledPinEyeRight7 = 26;
const int ledPinEyeRight8 = 37;
const int ledPinEyeRight9 = 35;
const int ledPinEyeRight10 = 33; 
const int ledPinEyeRight11 = 36;
const int ledPinEyeRight12 = 34;
//Left Eye
const int ledPinEyeLeft0 = 40;
const int ledPinEyeLeft1 = 52;
const int ledPinEyeLeft2 = 51; 
const int ledPinEyeLeft3 = 49; 
const int ledPinEyeLeft4 = 47; 
const int ledPinEyeLeft5 = 50; 
const int ledPinEyeLeft6 = 48; 
const int ledPinEyeLeft7 = 46; 
const int ledPinEyeLeft8 = 45; 
const int ledPinEyeLeft9 = 43;
const int ledPinEyeLeft10 = 41;
const int ledPinEyeLeft11 = 44; 
const int ledPinEyeLeft12 = 42;
//Back Power Button LED
const int ledBackPowerButton = 10;
//Plasma Relay
const int relayPinPlasmaGlobe = 23;
/* Globals */
unsigned long UptimeMillis;
int RCcontrolYPWM = 0;
int RCcontrolYPWMTop = 1973;
int RCcontrolYPWMMiddle = 1540;
int RCcontrolYPWMBottom = 1061;
int RCcontrolPWMYBuffer = 50;
int RCcontrolPWMXBuffer = 50;
int RCcontrolXPWM = 0;
int RCcontrolXPWMTop = 1972;
int RCcontrolXPWMMiddle = 1510;
int RCcontrolXPWMBottom = 1059; 
int motorLeftCurentPWM = 0;
int motorRightCurentPWM = 0;
int motorMinimumPWMforActuation = 95;
float motorBatteryVoltageNow = 0.0;
float motorBatteryVoltageHigh = 0.0;
float motorBatteryVoltageLow = 12.0;

// Hall Sensors
int wheelRotationsLeft = 0;
int WheelRotationsRight = 0;
int wheelRotationsLeftState = 0;
int WheelRotationsRightState = 0;

int LCDPageNumber = 0;
// automation state
int automationCurrentState = 0;
unsigned long automationCurrentStateElapsedMillis = 0;
unsigned long automationStateLastMillisMeasure = 0;
// Angel eye power button 
int backPowerButtonPWM = 0;
int backPowerButtonPulseRate = 70;
int backPowerButtonPulseSteps[] = {0,1,2,5,10,255,20,30,40,50,60,70,80,90,100,255,100,90,80,70,60,50,40,30,20,10,0};
int backPowerButtonPulsePosition = 0;
unsigned long backPowerButtonLastPulse = 0;

int CDSSensor1FrountValue = 0;
int CDSSensor2BackValue = 0;
int CDSSensor3LeftValue = 0;
int CDSSensor4RightValue = 0;
int CDSSensor5TopValue = 0;

//Not fully implemented yet
int RCcontrolYPWMRangeSpread = 0;
int RCcontrolYPWMTopDynamic = RCcontrolYPWMBottom;
int RCcontrolYPWMMiddleDynamic = 0;
int RCcontrolYPWMBottomDynamic = RCcontrolYPWMTop;
int RCcontrolXPWMRangeSpread = 0;
int RCcontrolXPWMTopDynamic = RCcontrolXPWMBottom;
int RCcontrolXPWMMiddleDynamic = 0;
int RCcontrolXPWMBottomDynamic = RCcontrolXPWMTop;
/* Enumerations */ 
const int displayVerbose = 1;
const int serialPrintRefreshRateMilli = 1000;
unsigned long serialPrintLastUpdateMilli = 0;
// automation state enumerations
const int stateUndefined = 0;
const int stateNoRCSignal = 5;
const int stateStationary = 10;
const int stateMoveForward = 20;
const int stateMoveForwardSteerLeft = 22;
const int stateMoveForwardSteerRight = 24;
const int stateMoveBackward = 30;
const int stateMoveBackwardSteerLeft = 32;
const int stateMoveBackwardSteerRight = 34;
const int stateMoveRotateLeft = 40;
const int stateMoveRotateRight = 50;

/* Library Objects */
//LiquidCrystal_I2C lcd(0x27,16,2); OLD LCD
LiquidCrystal_I2C lcd(0x27,20,4);
//Timers 
Timer timerEyeBlink;
Timer timeBackPowerButtonPulseRateCheck;

void setup() {
	Serial.begin(9600);
	lcd.init();
	lcd.backlight();

	setHBridgePins();
	pinMode(RCcontrolY, INPUT);
	pinMode(RCcontrolX, INPUT);
	pinMode(wheelLefttHallSensorPin, INPUT);
	pinMode(wheelRightHallSensorPin, INPUT);

	pinMode(ledBackPowerButton, OUTPUT);
	pinMode(relayPinPlasmaGlobe, OUTPUT);

	setEyePins();
	eyesOpen();
	setCDSPins();

	//Turn on plasma globe
	digitalWrite(relayPinPlasmaGlobe, LOW);

	int tickEvent = timerEyeBlink.every(11000, eyesBlink);
}
void loop() {
	UptimeMillis = millis();

	//Eyes
	timerEyeBlink.update();

	//Back Power Button
	backPowerButtonPulseRateChange();
	backPowerButtonPulse();

	//RC Controller
	RCReadControlPWM();
	RCAutomationStateSet();

	//Batter Monitoring
	motorBatteryAnalogVoltageDividerRead();

	//Wheel Rotations
	wheelHallSensorsRead();

	//CDS Monitoring
	CDSSensorRead();

	//LCD SCreen
	automationStateSerialPrint(displayVerbose);
}

//Setup Methods
void setCDSPins(){
	pinMode(CDSSensor1Frount, INPUT); 
	pinMode(CDSSensor2Back, INPUT); 
	pinMode(CDSSensor3Left, INPUT); 
	pinMode(CDSSensor4Right, INPUT); 
	pinMode(CDSSensor5Top, INPUT); 
}
void setHBridgePins(){
	pinMode(motorLeftPWM, OUTPUT); 
	pinMode(motorLeftBackward, OUTPUT); 
	pinMode(motorLeftForward, OUTPUT); 
	pinMode(motorRightPWM, OUTPUT); 
	pinMode(motorRightBackward, OUTPUT); 
	pinMode(motorRightForward, OUTPUT);
}
void setEyePins(){
	pinMode(ledPinEyeRight0, OUTPUT); 
	pinMode(ledPinEyeRight1, OUTPUT); 
	pinMode(ledPinEyeRight2, OUTPUT); 
	pinMode(ledPinEyeRight3, OUTPUT); 
	pinMode(ledPinEyeRight4, OUTPUT); 
	pinMode(ledPinEyeRight5, OUTPUT); 
	pinMode(ledPinEyeRight6, OUTPUT); 
	pinMode(ledPinEyeRight7, OUTPUT); 
	pinMode(ledPinEyeRight8, OUTPUT); 
	pinMode(ledPinEyeRight9, OUTPUT); 
	pinMode(ledPinEyeRight10, OUTPUT);  
	pinMode(ledPinEyeRight11, OUTPUT); 
	pinMode(ledPinEyeRight12, OUTPUT);

	pinMode(ledPinEyeLeft0, OUTPUT); 
	pinMode(ledPinEyeLeft1, OUTPUT); 
	pinMode(ledPinEyeLeft2, OUTPUT); 
	pinMode(ledPinEyeLeft3, OUTPUT); 
	pinMode(ledPinEyeLeft4, OUTPUT); 
	pinMode(ledPinEyeLeft5, OUTPUT); 
	pinMode(ledPinEyeLeft6, OUTPUT); 
	pinMode(ledPinEyeLeft7, OUTPUT); 
	pinMode(ledPinEyeLeft8, OUTPUT); 
	pinMode(ledPinEyeLeft9, OUTPUT); 
	pinMode(ledPinEyeLeft10, OUTPUT); 
	pinMode(ledPinEyeLeft11, OUTPUT); 
	pinMode(ledPinEyeLeft12, OUTPUT);
}

void CDSSensorRead(){

	CDSSensor1FrountValue = analogRead(CDSSensor1Frount);
	CDSSensor2BackValue = analogRead(CDSSensor2Back);
	CDSSensor3LeftValue = analogRead(CDSSensor3Left);
	CDSSensor4RightValue = analogRead(CDSSensor4Right);
	CDSSensor5TopValue = analogRead(CDSSensor5Top);

	//// We'll have a few threshholds, qualitatively determined
	//if (photocellReading < 10) {
	//	Serial.println(" - Dark");
	//} else if (photocellReading < 200) {
	//	Serial.println(" - Dim");
	//} else if (photocellReading < 500) {
	//	Serial.println(" - Light");
	//} else if (photocellReading < 800) {
	//	Serial.println(" - Bright");
	//} else {
	//	Serial.println(" - Very bright");
	//}
}

//Automation settings
void automationStateSet(int stateType){
	//Set time in current state
	if (stateType == automationCurrentState){
		//automationCurrentStateElapsedMillis = automationCurrentStateElapsedMillis + (UptimeMillis - automationStateLastMillisMeasure);
		automationCurrentStateElapsedMillis = UptimeMillis;
		automationStateLastMillisMeasure = UptimeMillis;
		wheelRotationsLeftState = wheelRotationsLeftState + (WheelRotationsRight - wheelRotationsLeftState);
		// 10							10							10						12
	}
	else //changed state. reset state dependant globals
	{
		automationCurrentStateElapsedMillis = 0;
		wheelRotationsLeftState = 0;
		WheelRotationsRightState = 0;
	}

	automationCurrentState = stateType;
}
String automationStateVerboseFormat(){
	switch (automationCurrentState){
	case stateUndefined:
		return("Undefined");
	case stateNoRCSignal:
		return("No RC Signal");
	case stateStationary:
		return("Stationary X:" + String(RCcontrolXPWM) + ", Y:" + String(RCcontrolYPWM));
	case stateMoveForward:
		return("Forward X:" + String(RCcontrolXPWM) + ", Y:" + String(RCcontrolYPWM));
	case stateMoveForwardSteerLeft:
		return("Forward Left X:" + String(RCcontrolXPWM) + ", Y:" + String(RCcontrolYPWM));
	case stateMoveForwardSteerRight:
		return("Forward Right X:" + String(RCcontrolXPWM) + ", Y:" + String(RCcontrolYPWM));
	case stateMoveBackward:
		return("Reverse X:" + String(RCcontrolXPWM) + ", Y:" + String(RCcontrolYPWM));
	case stateMoveBackwardSteerLeft:
		return("Reverse Left X:" + String(RCcontrolXPWM) + ", Y:" + String(RCcontrolYPWM));
	case stateMoveBackwardSteerRight:
		return("Reverse Right X:" + String(RCcontrolXPWM) + ", Y:" + String(RCcontrolYPWM));
	case stateMoveRotateLeft:
		return("Rotate Left X:" + String(RCcontrolXPWM) + ", Y:" + String(RCcontrolYPWM));
	case stateMoveRotateRight:
		return("Rotate Right X:" + String(RCcontrolXPWM) + ", Y:" + String(RCcontrolYPWM));
	}
}

//Back Power Button
void backPowerButtonPulse(){
	if(UptimeMillis - backPowerButtonLastPulse > backPowerButtonPulseRate) {
		if (backPowerButtonPulsePosition < 24){
			backPowerButtonLastPulse = UptimeMillis;
			backPowerButtonPulsePosition = backPowerButtonPulsePosition + 1;
		}
		else
		{ 
			backPowerButtonPulsePosition = 0;
		}
		analogWrite(ledBackPowerButton, backPowerButtonPulseSteps[backPowerButtonPulsePosition]);
	}
}
void backPowerButtonPulseRateChange(){
	if ((automationCurrentState == stateMoveForward) || (automationCurrentState == stateMoveForwardSteerLeft) || (automationCurrentState == stateMoveForwardSteerRight)
		|| (automationCurrentState == stateMoveBackward) || (automationCurrentState == stateMoveBackwardSteerLeft) || (automationCurrentState == stateMoveBackwardSteerRight)
		|| (automationCurrentState == stateMoveRotateLeft) || (automationCurrentState == stateMoveRotateRight)){
			//check time spent moving to determin speed up
			if ((automationCurrentStateElapsedMillis > 2000) && (automationCurrentStateElapsedMillis < 5000)){
				backPowerButtonPulseRate = 50; 

			}
			else if ((automationCurrentStateElapsedMillis > 5000) && (automationCurrentStateElapsedMillis < 15000)){
				backPowerButtonPulseRate = 25;
			}
			else if (automationCurrentStateElapsedMillis > 15000){
				backPowerButtonPulseRate = 5; // exhausted
			}
	}
	else if ((automationCurrentState == stateStationary) ||  (automationCurrentState == stateNoRCSignal)){
		//Check time spent resting to cool down
		if ((automationCurrentStateElapsedMillis > 2000) && (automationCurrentStateElapsedMillis < 5000)){
			backPowerButtonPulseRate = 50; 

		}
		else if ((automationCurrentStateElapsedMillis > 5000) && (automationCurrentStateElapsedMillis < 30000)){
			backPowerButtonPulseRate = 80; // normal pulse
		}
		else if (automationCurrentStateElapsedMillis > 30000){
			backPowerButtonPulseRate = 170; // resting
		}
	}
}

//Eyes
void eyesBlink(){
	digitalWrite(ledPinEyeLeft0, LOW);
	digitalWrite(ledPinEyeLeft1, LOW);
	digitalWrite(ledPinEyeRight0, LOW);
	digitalWrite(ledPinEyeRight1, LOW);
	delay(30);
	digitalWrite(ledPinEyeRight12, LOW);
	digitalWrite(ledPinEyeLeft12, LOW);
	delay(30);
	digitalWrite(ledPinEyeLeft2, LOW);
	digitalWrite(ledPinEyeLeft11, LOW);
	digitalWrite(ledPinEyeRight2, LOW);
	digitalWrite(ledPinEyeRight11, LOW);
	delay(30);
	digitalWrite(ledPinEyeLeft3, LOW);
	digitalWrite(ledPinEyeLeft10, LOW);
	digitalWrite(ledPinEyeRight3, LOW);
	digitalWrite(ledPinEyeRight10, LOW);
	delay(30);
	digitalWrite(ledPinEyeLeft4, LOW);
	digitalWrite(ledPinEyeLeft9, LOW);
	digitalWrite(ledPinEyeRight4, LOW);
	digitalWrite(ledPinEyeRight9, LOW);
	delay(20);
	digitalWrite(ledPinEyeLeft5, LOW);
	digitalWrite(ledPinEyeLeft8, LOW);
	digitalWrite(ledPinEyeRight5, LOW);
	digitalWrite(ledPinEyeRight8, LOW);
	delay(20);
	digitalWrite(ledPinEyeLeft6, LOW);
	digitalWrite(ledPinEyeLeft7, LOW);
	digitalWrite(ledPinEyeRight6, LOW);
	digitalWrite(ledPinEyeRight7, LOW);

	//Lid Up
	delay(20);
	digitalWrite(ledPinEyeLeft6, HIGH);
	digitalWrite(ledPinEyeLeft7, HIGH);
	digitalWrite(ledPinEyeRight6, HIGH);
	digitalWrite(ledPinEyeRight7, HIGH);
	delay(20);
	digitalWrite(ledPinEyeLeft5, HIGH);
	digitalWrite(ledPinEyeLeft8, HIGH);
	digitalWrite(ledPinEyeRight5, HIGH);
	digitalWrite(ledPinEyeRight8, HIGH);
	delay(20);
	digitalWrite(ledPinEyeLeft4, HIGH);
	digitalWrite(ledPinEyeLeft9, HIGH);
	digitalWrite(ledPinEyeRight4, HIGH);
	digitalWrite(ledPinEyeRight9, HIGH);
	delay(20);
	digitalWrite(ledPinEyeLeft3, HIGH);
	digitalWrite(ledPinEyeLeft10, HIGH);
	digitalWrite(ledPinEyeRight3, HIGH);
	digitalWrite(ledPinEyeRight10, HIGH);
	delay(20);
	digitalWrite(ledPinEyeLeft2, HIGH);
	digitalWrite(ledPinEyeLeft11, HIGH);
	digitalWrite(ledPinEyeRight2, HIGH);
	digitalWrite(ledPinEyeRight11, HIGH);
	delay(20);
	digitalWrite(ledPinEyeLeft12, HIGH);
	digitalWrite(ledPinEyeRight12, HIGH);
	delay(20);
	digitalWrite(ledPinEyeLeft0, HIGH);
	digitalWrite(ledPinEyeLeft1, HIGH);
	digitalWrite(ledPinEyeRight0, HIGH);
	digitalWrite(ledPinEyeRight1, HIGH);
}
void eyesOpen()
{
	digitalWrite(ledPinEyeRight0, HIGH);
	digitalWrite(ledPinEyeRight1, HIGH);
	digitalWrite(ledPinEyeRight2, HIGH);
	digitalWrite(ledPinEyeRight3, HIGH);
	digitalWrite(ledPinEyeRight4, HIGH);
	digitalWrite(ledPinEyeRight5, HIGH);
	digitalWrite(ledPinEyeRight6, HIGH);
	digitalWrite(ledPinEyeRight7, HIGH);
	digitalWrite(ledPinEyeRight8, HIGH);
	digitalWrite(ledPinEyeRight9, HIGH);
	digitalWrite(ledPinEyeRight10, HIGH);
	digitalWrite(ledPinEyeRight11, HIGH);
	digitalWrite(ledPinEyeRight12, HIGH);

	digitalWrite(ledPinEyeLeft0, HIGH);
	digitalWrite(ledPinEyeLeft1, HIGH);
	digitalWrite(ledPinEyeLeft2, HIGH);
	digitalWrite(ledPinEyeLeft3, HIGH);
	digitalWrite(ledPinEyeLeft4, HIGH);
	digitalWrite(ledPinEyeLeft5, HIGH);
	digitalWrite(ledPinEyeLeft6, HIGH);
	digitalWrite(ledPinEyeLeft7, HIGH);
	digitalWrite(ledPinEyeLeft8, HIGH);
	digitalWrite(ledPinEyeLeft9, HIGH);
	digitalWrite(ledPinEyeLeft10, HIGH);
	digitalWrite(ledPinEyeLeft11, HIGH);
	digitalWrite(ledPinEyeLeft12, HIGH);
}
void eyesClose(){
	digitalWrite(ledPinEyeRight0, LOW);
	digitalWrite(ledPinEyeRight1, LOW);
	digitalWrite(ledPinEyeRight2, LOW);
	digitalWrite(ledPinEyeRight3, LOW);
	digitalWrite(ledPinEyeRight4, LOW);
	digitalWrite(ledPinEyeRight5, LOW);
	digitalWrite(ledPinEyeRight6, LOW);
	digitalWrite(ledPinEyeRight7, LOW);
	digitalWrite(ledPinEyeRight8, LOW);
	digitalWrite(ledPinEyeRight9, LOW);
	digitalWrite(ledPinEyeRight10, LOW);
	digitalWrite(ledPinEyeRight11, LOW);
	digitalWrite(ledPinEyeRight12, LOW);

	digitalWrite(ledPinEyeLeft0, LOW);
	digitalWrite(ledPinEyeLeft1, LOW);
	digitalWrite(ledPinEyeLeft2, LOW);
	digitalWrite(ledPinEyeLeft3, LOW);
	digitalWrite(ledPinEyeLeft4, LOW);
	digitalWrite(ledPinEyeLeft5, LOW);
	digitalWrite(ledPinEyeLeft6, LOW);
	digitalWrite(ledPinEyeLeft7, LOW);
	digitalWrite(ledPinEyeLeft8, LOW);
	digitalWrite(ledPinEyeLeft9, LOW);
	digitalWrite(ledPinEyeLeft10, LOW);
	digitalWrite(ledPinEyeLeft11, LOW);
	digitalWrite(ledPinEyeLeft12, LOW);
}

//LCD output
void automationStateSerialPrint(int displayType){
	if(UptimeMillis - serialPrintLastUpdateMilli > serialPrintRefreshRateMilli) {
		serialPrintLastUpdateMilli = UptimeMillis; 
		if (displayType == displayVerbose) {
			//Serial.println(automationStateVerboseFormat());
			//LCDdisplay(automationStateVerboseFormat());
			LCDdisplayOverride();
		}
	}
}
void LCDdisplayOverride(){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("CDS PhotoCell Values");

	lcd.setCursor(0,1);
	lcd.print("Top  ");
	lcd.print(CDSSensor5TopValue);

	lcd.setCursor(0,2);
	lcd.print("Frnt ");
	lcd.print(CDSSensor1FrountValue);
	lcd.print(" Back  ");
	lcd.print(CDSSensor2BackValue);

	lcd.setCursor(0,3);
	lcd.print("Left ");
	lcd.print(CDSSensor3LeftValue);
	lcd.print(" Right ");
	lcd.print(CDSSensor4RightValue);
}
void LCDdisplay(String state){
	lcd.clear();

	if (automationCurrentState == stateMoveForward || automationCurrentState == stateMoveForwardSteerLeft || automationCurrentState == stateMoveForwardSteerRight){
		lcd.setCursor(0,0);
		lcd.print("MotorPWM ");
		lcd.print(motorLeftCurentPWM);
		lcd.print("/");
		lcd.print(motorRightCurentPWM);

		lcd.setCursor(0,1);
		lcd.print("Vlt ");
		lcd.print((int)motorBatteryVoltageHigh);
		lcd.print("/");
		lcd.print(motorBatteryVoltageNow);
		lcd.print("/");
		lcd.print((int)motorBatteryVoltageLow);

		lcd.setCursor(0,2);
		lcd.print("RC PMM Y/X ");
		lcd.setCursor(0,1);
		lcd.print(RCcontrolYPWM);
		lcd.print("/");
		lcd.print(RCcontrolXPWM);
	}
	else
	{
		switch (LCDPageNumber){
		case 1:
			lcd.setCursor(0, 0);
			lcd.print(state);

			lcd.setCursor(0, 1);
			lcd.print("State Time ");
			lcd.print(automationCurrentStateElapsedMillis);
			break;

		case 2:
			lcd.setCursor(0,0);
			lcd.print("MotorPWM ");
			lcd.print(motorLeftCurentPWM);
			lcd.print("/");
			lcd.print(motorRightCurentPWM);

			lcd.setCursor(0,1);
			lcd.print("Vlt ");
			lcd.print((int)motorBatteryVoltageHigh);
			lcd.print("/");
			lcd.print(motorBatteryVoltageNow);
			lcd.print("/");
			lcd.print((int)motorBatteryVoltageLow);

			lcd.setCursor(0,2);
			lcd.print("L/R Hall ");
			lcd.print(wheelHallSensorLeft);
			lcd.print("/");
			lcd.print(wheelHallSensorRight);
			break;

		case 3:
			lcd.setCursor(0,0);
			lcd.print("CDS Frount ");
			lcd.print(CDSSensor1FrountValue);

			lcd.setCursor(0,1);
			lcd.print("CDS Back ");
			lcd.print(CDSSensor2BackValue);

			lcd.setCursor(0,2);
			lcd.print("CDS Left ");
			lcd.print(CDSSensor3LeftValue);

			lcd.setCursor(0,3);
			lcd.print("CDS Right ");
			lcd.print(CDSSensor4RightValue);

			//lcd.print(" CDS Top ");
			//lcd.print(CDSSensor5TopValue);
			break;

		default://Last page	
			lcd.setCursor(0,0);
			lcd.print("RC PMM Y/X ");
			lcd.setCursor(0,1);
			lcd.print(RCcontrolYPWM);
			lcd.print("/");
			lcd.print(RCcontrolXPWM);

			lcd.setCursor(0,2);
			lcd.print("Y ");
			lcd.print(RCcontrolYPWMTop);
			lcd.print("/");
			lcd.print(RCcontrolYPWM);
			lcd.print("/");
			lcd.print(RCcontrolYPWMBottom);

			lcd.setCursor(0,3);
			lcd.print("X ");
			lcd.print(RCcontrolXPWMTop);
			lcd.print("/");
			lcd.print(RCcontrolXPWM);
			lcd.print("/");
			lcd.print(RCcontrolXPWMBottom);

			LCDPageNumber = 0; 
			break;
		}
		LCDPageNumber++;
	}
}

//Read RC controller
void RCReadControlPWM()
{
	//RCcontrolXPWM = pulseIn(RCcontrolX, HIGH, 25000);
	RCcontrolXPWM = pulseIn(RCcontrolX, HIGH);
	if (RCcontrolXPWM != 0){
		RCcontrolXAutoAdjustLimits(RCcontrolXPWM);
		RCcontrolXPWM = RCcontrolXPWM == 0 ? RCcontrolXPWMMiddle : RCcontrolXPWM;
		RCcontrolXPWM = constrain(RCcontrolXPWM, RCcontrolXPWMBottom, RCcontrolXPWMTop);
	}

	//RCcontrolYPWM = pulseIn(RCcontrolY, HIGH, 25000);
	RCcontrolYPWM = pulseIn(RCcontrolY, HIGH);
	if (RCcontrolYPWM != 0){
		RCcontrolYAutoAdjustLimits(RCcontrolYPWM);
		RCcontrolYPWM = RCcontrolYPWM == 0 ? RCcontrolYPWMMiddle : RCcontrolYPWM;
		RCcontrolYPWM = constrain(RCcontrolYPWM, RCcontrolYPWMBottom, RCcontrolYPWMTop);
	}
}
void RCAutomationStateSet(){
	bool RChasSignal = false;

	if ((RCcontrolYPWM > 0) && (RCcontrolXPWM > 0))
	{
		RChasSignal = true;
	}
	else
	{
		RChasSignal = false;
	}


	if (RChasSignal == true){
		//lcd.setBacklight(1);
		RCActivateState();
	}
	if (RChasSignal == false){
		actionStationary();
		automationStateSet(stateNoRCSignal);
	}
}
void RCcontrolXAutoAdjustLimits(int RCcontrolX){
	RCcontrolXPWMTopDynamic = max(RCcontrolXPWM, RCcontrolXPWMTopDynamic);
	RCcontrolXPWMBottomDynamic = min(RCcontrolXPWM, RCcontrolXPWMBottomDynamic);

	if (RCcontrolXPWMTop < RCcontrolXPWMTopDynamic){
	}
	if (RCcontrolXPWMBottom > RCcontrolXPWMBottomDynamic){
	}

	RCcontrolXPWMRangeSpread = RCcontrolXPWMTop - RCcontrolXPWMBottom;
	RCcontrolXPWMMiddleDynamic = (int)(RCcontrolXPWMRangeSpread * .5) + RCcontrolXPWMBottom;
}
void RCcontrolYAutoAdjustLimits(int RCcontrolY){
	RCcontrolYPWMTopDynamic = max(RCcontrolYPWM, RCcontrolYPWMTopDynamic);
	RCcontrolYPWMBottomDynamic = min(RCcontrolYPWM, RCcontrolYPWMBottomDynamic);

	if (RCcontrolYPWMTop < RCcontrolYPWMTopDynamic){
	}
	if (RCcontrolYPWMBottom > RCcontrolYPWMBottomDynamic){
	}

	RCcontrolYPWMRangeSpread = RCcontrolYPWMTop - RCcontrolYPWMBottom;
	RCcontrolYPWMMiddleDynamic = (int)(RCcontrolYPWMRangeSpread * .5) + RCcontrolYPWMBottom;
}
void RCActivateState(){
	if(RCcontrolIsMoveForwardRequested()) {
		actionMoveForward();
	}
	else if(RCControlIsMoveBackwardRequested()) {
		actionMoveBackward();
	} 
	else if(RCcontrolIsMoveRotateLeftRequested()){

		actionMoveRotateLeft();
	}
	else if(RCcontrolIsMoveRotateRightRequested()){
		actionMoveRotateRight();
	}
	else{
		actionStationary();
	}
}

// Movement trigger tolerances
boolean RCcontrolIsMoveForwardRequested()
{
	return RCcontrolYPWM < (RCcontrolYPWMMiddle - RCcontrolPWMYBuffer);
}
boolean RCControlIsMoveBackwardRequested()
{
	return RCcontrolYPWM > (RCcontrolYPWMMiddle + RCcontrolPWMYBuffer); 
}
boolean RCcontrolIsMoveRotateLeftRequested(){
	return RCcontrolXPWM < (RCcontrolXPWMMiddle - RCcontrolPWMXBuffer);  
}
boolean RCcontrolIsMoveRotateRightRequested(){
	return RCcontrolXPWM > (RCcontrolXPWMMiddle + RCcontrolPWMXBuffer);
}

// Movement PWM settings and actuation 
void actionMoveForward()
{
	automationStateSet(stateMoveForward);
	/*Why negative numbers? need abs()*/
	int motorPWM = abs(map(RCcontrolYPWM, RCcontrolYPWMMiddle, RCcontrolYPWMTop, 0, 255));
	motorPWM = constrain(motorPWM, motorMinimumPWMforActuation, 255);
	actionMoveSteer(motorPWM, stateMoveForward);

	digitalWrite(motorLeftForward, LOW);
	digitalWrite(motorLeftBackward, HIGH);
	digitalWrite(motorRightForward, LOW);
	digitalWrite(motorRightBackward, HIGH);

	analogWrite(motorLeftPWM, motorLeftCurentPWM);
	analogWrite(motorRightPWM, motorRightCurentPWM);
}
void actionMoveBackward()
{
	automationStateSet(stateMoveBackward);
	/*Why negative numbers? need abs()*/
	int motorPWM = abs(map(RCcontrolXPWM, RCcontrolXPWMMiddle, RCcontrolXPWMTop, 0, 255));
	motorPWM = constrain(motorPWM, motorMinimumPWMforActuation, 255);
	actionMoveSteer(motorPWM, stateMoveForward);

	digitalWrite(motorLeftForward, HIGH);
	digitalWrite(motorLeftBackward, LOW);
	digitalWrite(motorRightForward, HIGH);
	digitalWrite(motorRightBackward, LOW);

	analogWrite(motorLeftPWM, motorLeftCurentPWM);
	analogWrite(motorRightPWM, motorRightCurentPWM);
}
void actionMoveRotateLeft(){
	automationStateSet(stateMoveRotateLeft);
	int mtrTurnPWM = abs(map(RCcontrolXPWM, RCcontrolXPWMMiddle, RCcontrolXPWMTop, 0, 255)); 

	digitalWrite(motorLeftForward, HIGH);
	digitalWrite(motorLeftBackward, LOW);
	digitalWrite(motorRightForward, LOW);
	digitalWrite(motorRightBackward, HIGH);

	analogWrite(motorLeftPWM, mtrTurnPWM);
	analogWrite(motorRightPWM, mtrTurnPWM);
}
void actionMoveRotateRight(){
	automationStateSet(stateMoveRotateRight);
	int mtrTurnPWM = abs(map(RCcontrolXPWM, RCcontrolXPWMMiddle, RCcontrolXPWMBottom, 0, 255));

	digitalWrite(motorLeftForward, LOW);
	digitalWrite(motorLeftBackward, HIGH);
	digitalWrite(motorRightForward, HIGH);
	digitalWrite(motorRightBackward, LOW);

	analogWrite(motorLeftPWM, mtrTurnPWM);
	analogWrite(motorRightPWM, mtrTurnPWM);
}
void actionMoveSteer(int motorPWM, int state)
{
	motorLeftCurentPWM = motorPWM;
	motorRightCurentPWM = motorPWM;
	int mtrTurnPWM = 0;

	if (state == stateMoveForward)
	{
		if (RCcontrolIsMoveRotateLeftRequested())
		{
			automationStateSet(stateMoveForwardSteerLeft);
			mtrTurnPWM = abs(map(RCcontrolXPWM, RCcontrolXPWMMiddle, RCcontrolXPWMBottom, 0, 255));
			motorLeftCurentPWM = constrain(mtrTurnPWM + motorLeftCurentPWM, 0, 255);
			motorRightCurentPWM =  constrain((mtrTurnPWM * -1 ) + motorRightCurentPWM, 0,255);
		}
		else if (RCcontrolIsMoveRotateRightRequested())
		{
			automationStateSet(stateMoveForwardSteerRight);
			mtrTurnPWM = abs(map(RCcontrolXPWM, RCcontrolXPWMMiddle, RCcontrolXPWMTop, 0, 255)); 
			motorLeftCurentPWM = constrain((mtrTurnPWM * -1 ) + motorLeftCurentPWM, 0, 255);
			motorRightCurentPWM =  constrain(mtrTurnPWM + motorRightCurentPWM, 0, 255);
		}
	}
	else if (state = stateMoveBackward)
	{
		if (RCcontrolIsMoveRotateLeftRequested())
		{
			automationStateSet(stateMoveBackwardSteerLeft);
			mtrTurnPWM = abs(map(RCcontrolXPWM, RCcontrolXPWMMiddle, RCcontrolXPWMBottom, 0, 255));
			motorLeftCurentPWM = constrain(mtrTurnPWM + motorLeftCurentPWM, 0, 255);
			motorRightCurentPWM =  constrain((mtrTurnPWM * -1 ) + motorRightCurentPWM, 0, 255);
		}
		else if (RCcontrolIsMoveRotateRightRequested())
		{
			automationStateSet(stateMoveBackwardSteerRight);
			mtrTurnPWM = abs(map(RCcontrolXPWM, RCcontrolXPWMMiddle, RCcontrolXPWMTop, 0, 255)); 
			motorLeftCurentPWM = constrain((mtrTurnPWM * -1 ) + motorLeftCurentPWM, 0, 255);
			motorRightCurentPWM =  constrain(mtrTurnPWM + motorRightCurentPWM, 0, 255);
		}
	}
}
void actionStationary()
{
	automationStateSet(stateStationary);
	motorLeftCurentPWM = 0;
	motorRightCurentPWM = 0;
	digitalWrite(motorLeftBackward, LOW);
	digitalWrite(motorLeftForward, LOW);
	digitalWrite(motorRightForward, LOW);
	digitalWrite(motorRightBackward, LOW);
}

//Additional sensors
void motorBatteryAnalogVoltageDividerRead()
{
	float sensorValue = analogRead(AnalogVoltageDividerPin);
	motorBatteryVoltageNow = (sensorValue / 4.092) * .1;
	motorBatteryVoltageHigh = max(motorBatteryVoltageNow, motorBatteryVoltageHigh);
	motorBatteryVoltageLow = min(motorBatteryVoltageNow, motorBatteryVoltageLow);
}
void wheelHallSensorsRead()
{
	wheelHallSensorLeft = digitalRead(wheelLefttHallSensorPin);
	wheelHallSensorRight = digitalRead(wheelRightHallSensorPin);
}