/*
NEEDS FIXED:
TODO:
Fix Right rotation PWM clipping on the extreme right PWM scale
rotate one pass of hall sensor to check rotation angle: approx 90 degrees
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

/* Pin Assignments Digital */
//RC Controller
const int pinRCcontrollerY = 9;
const int pinRCcontrollerX = 8;
//H-Bridge
const int pinHBridgeLeftForward = 3;
const int pinHBridgeLeftBackward = 4;
const int pinHBridgeLeftPWM = 5; //I think this is wired backwards...
const int pinHBridgeRightForward = 7;
const int pinHBridgeRightBackward = 6; 
const int pinHBridgeRightPWM = 2; //I think this is wired backwards...

// Right Eye
const int pinLEDEyeRight0 = 32;
const int pinLEDEyeRight1 = 38; 
const int pinLEDEyeRight2 = 31;
const int pinLEDEyeRight3 = 29;
const int pinLEDEyeRight4 = 27;
const int pinLEDEyeRight5 = 30;
const int pinLEDEyeRight6 = 28;
const int pinLEDEyeRight7 = 26;
const int pinLEDEyeRight8 = 37;
const int pinLEDEyeRight9 = 35;
const int pinLEDEyeRight10 = 33; 
const int pinLEDEyeRight11 = 36;
const int pinLEDEyeRight12 = 34;
//Left Eye
const int pinLEDEyeLeft0 = 44;//40
const int pinLEDEyeLeft1 = 52;
const int pinLEDEyeLeft2 = 51; 
const int pinLEDEyeLeft3 = 49; 
const int pinLEDEyeLeft4 = 47; 
const int pinLEDEyeLeft5 = 50; 
const int pinLEDEyeLeft6 = 48; 
const int pinLEDEyeLeft7 = 46; 
const int pinLEDEyeLeft8 = 45; 
const int pinLEDEyeLeft9 = 43;
const int pinLEDEyeLeft10 = 44;//41
const int pinLEDEyeLeft11 = 44; 
const int pinLEDEyeLeft12 = 42;
//Back Power Button LED
const int pinLEDBackPowerButton = 10;
//Plasma Relay
const int pinRelayPlasmaGlobe = 23;
//Hall Sensors
const int pinHallSensorLeftWheel = 40;
const int pinHallSensorRightWheel = 41;

/* Pin Assignments Analog */
//Voltage Sensors
const int pinAnalogVoltageDivider = 0;
//CDS Sensors
const int pinAnalogCDSSensor1Frount = 1;
const int pinAnalogCDSSensor2Back = 2;
const int pinAnalogCDSSensor3Left = 3;
const int pinAnalogCDSSensor4Right = 4;
const int pinAnalogCDSSensor5Top = 5;


/* Enumerations */ 
const int displayVerbose = 1;
const int serialPrintRefreshRateMilli = 2000;
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
int wheelHallSensorLeft = 0;
int wheelHallSensorRight = 0;
int wheelRotationsLeft = 0;
int WheelRotationsRight = 0;
int wheelRotationsLeftState = 0;
int wheelRotationsRightState = 0;

//LCD Screen
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

// CDS Photocells
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

// Automated Behavior
unsigned long automatedBehaviorLastUpdateMilli = 0;
int automatedBehaviorRefreshRateMilli = 500;


void setup() {
	Serial.begin(9600);
	lcd.init();
	lcd.backlight();

	setHBridgePins();
	pinMode(pinRCcontrollerY, INPUT);
	pinMode(pinRCcontrollerX, INPUT);
	pinMode(pinHallSensorLeftWheel, INPUT);
	pinMode(pinHallSensorRightWheel, INPUT);

	pinMode(pinLEDBackPowerButton, OUTPUT);
	pinMode(pinRelayPlasmaGlobe, OUTPUT);

	setEyePins();
	eyesOpen();
	setCDSPins();

	//Turn on plasma globe
	//digitalWrite(pinRelayPlasmaGlobe, LOW);

	int tickEvent = timerEyeBlink.every(11000, eyesBlink);

	hallAlignWheels();
}
void loop() {
	UptimeMillis = millis();
	//RC Controller
	//RCReadControlPWM();
	//RCAutomationStateSet();

	//Eyes
	timerEyeBlink.update();
	//Back Power Button
	backPowerButtonPulseRateChange();
	backPowerButtonPulse();
	//Wheel Rotations
	wheelHallSensorsRead();
	//CDS Monitoring
	CDSSensorRead();

	//FaceTheLight();

	//Battery Monitoring
	motorBatteryAnalogVoltageDividerRead();
	//LCD SCreen
	automationStateDisplay(displayVerbose);
}
void hallAlignWheels(){
	do
	{
		actionMoveRotateLeftWheelForward(120);
		actionMoveRotateRightWheelBackward(120);
		wheelHallSensorsRead();
	} while ((wheelHallSensorRight == 0) && (wheelHallSensorLeft == 0));
	actionStopMovment();
}
/* Behaviors */
// Rotate twords light
void FaceTheLight(){
	//if(UptimeMillis - automatedBehaviorLastUpdateMilli > automatedBehaviorRefreshRateMilli) {
	//automatedBehaviorLastUpdateMilli = UptimeMillis; 

	//if (CDSSensor3LeftValue > CDSSensor4RightValue){
	//	//Rotate Left
	//	actionMoveRotateLeft(130);
	//}
	//else if (CDSSensor4RightValue > CDSSensor3LeftValue)  {
	//	//Rotate Right
	//	actionMoveRotateRight(130);
	//}
	//else{
	//	actionStopMovment();
	//}

	//store all four values in an array
	int CDSValues[3];
	CDSValues[0] = CDSSensor1FrountValue;
	CDSValues[1] = CDSSensor2BackValue;
	CDSValues[2] = CDSSensor3LeftValue;
	CDSValues[3] = CDSSensor4RightValue;
	// Read all four values
	// Find the highest value of the four
	int index = getIndexOfMaximumValue(CDSValues, 4);
	Serial.println(index);
	// if frount then do nothing, else start turning
	if (index != 0){
		if (index == 3){ //Turn left

		}
		else if (index == 4){ // Turn right

		}
		else if (index == 5){ // Turn right heading twords backwards photocell

		}
	}

	// keep turning untill frount registers simalr value to previously highest value is found within 20 ints
	//}
}
int getIndexOfMaximumValue(int* array, int size){
	int maxIndex = 0;
	int max = array[maxIndex];
	for (int i=1; i < size; i++){
		//Serial.println(i);
		if (max < array[i]){

			max = array[i];
			maxIndex = i;
		}
	}
	return maxIndex;
}


//Setup Methods
void setCDSPins(){
	pinMode(pinAnalogCDSSensor1Frount, INPUT); 
	pinMode(pinAnalogCDSSensor2Back, INPUT); 
	pinMode(pinAnalogCDSSensor3Left, INPUT); 
	pinMode(pinAnalogCDSSensor4Right, INPUT); 
	pinMode(pinAnalogCDSSensor5Top, INPUT); 
}
void setHBridgePins(){
	pinMode(pinHBridgeLeftPWM, OUTPUT); 
	pinMode(pinHBridgeLeftBackward, OUTPUT); 
	pinMode(pinHBridgeLeftForward, OUTPUT); 
	pinMode(pinHBridgeRightPWM, OUTPUT); 
	pinMode(pinHBridgeRightBackward, OUTPUT); 
	pinMode(pinHBridgeRightForward, OUTPUT);
}
void setEyePins(){
	pinMode(pinLEDEyeRight0, OUTPUT); 
	pinMode(pinLEDEyeRight1, OUTPUT); 
	pinMode(pinLEDEyeRight2, OUTPUT); 
	pinMode(pinLEDEyeRight3, OUTPUT); 
	pinMode(pinLEDEyeRight4, OUTPUT); 
	pinMode(pinLEDEyeRight5, OUTPUT); 
	pinMode(pinLEDEyeRight6, OUTPUT); 
	pinMode(pinLEDEyeRight7, OUTPUT); 
	pinMode(pinLEDEyeRight8, OUTPUT); 
	pinMode(pinLEDEyeRight9, OUTPUT); 
	pinMode(pinLEDEyeRight10, OUTPUT);  
	pinMode(pinLEDEyeRight11, OUTPUT); 
	pinMode(pinLEDEyeRight12, OUTPUT);

	pinMode(pinLEDEyeLeft0, OUTPUT); 
	pinMode(pinLEDEyeLeft1, OUTPUT); 
	pinMode(pinLEDEyeLeft2, OUTPUT); 
	pinMode(pinLEDEyeLeft3, OUTPUT); 
	pinMode(pinLEDEyeLeft4, OUTPUT); 
	pinMode(pinLEDEyeLeft5, OUTPUT); 
	pinMode(pinLEDEyeLeft6, OUTPUT); 
	pinMode(pinLEDEyeLeft7, OUTPUT); 
	pinMode(pinLEDEyeLeft8, OUTPUT); 
	pinMode(pinLEDEyeLeft9, OUTPUT); 
	pinMode(pinLEDEyeLeft10, OUTPUT); 
	pinMode(pinLEDEyeLeft11, OUTPUT); 
	pinMode(pinLEDEyeLeft12, OUTPUT);
}

//Automation settings
void automationStateSet(int stateType){
	//Set time in current 
	if (automationCurrentState == stateType){
		automationCurrentStateElapsedMillis = automationCurrentStateElapsedMillis + (UptimeMillis - automationStateLastMillisMeasure);

		wheelRotationsLeftState = wheelRotationsLeftState + (WheelRotationsRight - wheelRotationsLeftState);
		wheelRotationsRightState = wheelRotationsRightState + (WheelRotationsRight - wheelRotationsRightState);

		automationStateLastMillisMeasure = UptimeMillis;

	}
	else //changed state. reset state dependant globals
	{
		automationCurrentStateElapsedMillis = 0;
		wheelRotationsLeftState = 0;
		wheelRotationsRightState = 0;
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
		analogWrite(pinLEDBackPowerButton, backPowerButtonPulseSteps[backPowerButtonPulsePosition]);
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
	digitalWrite(pinLEDEyeLeft0, LOW);
	digitalWrite(pinLEDEyeLeft1, LOW);
	digitalWrite(pinLEDEyeRight0, LOW);
	digitalWrite(pinLEDEyeRight1, LOW);
	delay(30);
	digitalWrite(pinLEDEyeRight12, LOW);
	digitalWrite(pinLEDEyeLeft12, LOW);
	delay(30);
	digitalWrite(pinLEDEyeLeft2, LOW);
	digitalWrite(pinLEDEyeLeft11, LOW);
	digitalWrite(pinLEDEyeRight2, LOW);
	digitalWrite(pinLEDEyeRight11, LOW);
	delay(30);
	digitalWrite(pinLEDEyeLeft3, LOW);
	digitalWrite(pinLEDEyeLeft10, LOW);
	digitalWrite(pinLEDEyeRight3, LOW);
	digitalWrite(pinLEDEyeRight10, LOW);
	delay(30);
	digitalWrite(pinLEDEyeLeft4, LOW);
	digitalWrite(pinLEDEyeLeft9, LOW);
	digitalWrite(pinLEDEyeRight4, LOW);
	digitalWrite(pinLEDEyeRight9, LOW);
	delay(20);
	digitalWrite(pinLEDEyeLeft5, LOW);
	digitalWrite(pinLEDEyeLeft8, LOW);
	digitalWrite(pinLEDEyeRight5, LOW);
	digitalWrite(pinLEDEyeRight8, LOW);
	delay(20);
	digitalWrite(pinLEDEyeLeft6, LOW);
	digitalWrite(pinLEDEyeLeft7, LOW);
	digitalWrite(pinLEDEyeRight6, LOW);
	digitalWrite(pinLEDEyeRight7, LOW);

	//Lid Up
	delay(20);
	digitalWrite(pinLEDEyeLeft6, HIGH);
	digitalWrite(pinLEDEyeLeft7, HIGH);
	digitalWrite(pinLEDEyeRight6, HIGH);
	digitalWrite(pinLEDEyeRight7, HIGH);
	delay(20);
	digitalWrite(pinLEDEyeLeft5, HIGH);
	digitalWrite(pinLEDEyeLeft8, HIGH);
	digitalWrite(pinLEDEyeRight5, HIGH);
	digitalWrite(pinLEDEyeRight8, HIGH);
	delay(20);
	digitalWrite(pinLEDEyeLeft4, HIGH);
	digitalWrite(pinLEDEyeLeft9, HIGH);
	digitalWrite(pinLEDEyeRight4, HIGH);
	digitalWrite(pinLEDEyeRight9, HIGH);
	delay(20);
	digitalWrite(pinLEDEyeLeft3, HIGH);
	digitalWrite(pinLEDEyeLeft10, HIGH);
	digitalWrite(pinLEDEyeRight3, HIGH);
	digitalWrite(pinLEDEyeRight10, HIGH);
	delay(20);
	digitalWrite(pinLEDEyeLeft2, HIGH);
	digitalWrite(pinLEDEyeLeft11, HIGH);
	digitalWrite(pinLEDEyeRight2, HIGH);
	digitalWrite(pinLEDEyeRight11, HIGH);
	delay(20);
	digitalWrite(pinLEDEyeLeft12, HIGH);
	digitalWrite(pinLEDEyeRight12, HIGH);
	delay(20);
	digitalWrite(pinLEDEyeLeft0, HIGH);
	digitalWrite(pinLEDEyeLeft1, HIGH);
	digitalWrite(pinLEDEyeRight0, HIGH);
	digitalWrite(pinLEDEyeRight1, HIGH);
}
void eyesOpen()
{
	digitalWrite(pinLEDEyeRight0, HIGH);
	digitalWrite(pinLEDEyeRight1, HIGH);
	digitalWrite(pinLEDEyeRight2, HIGH);
	digitalWrite(pinLEDEyeRight3, HIGH);
	digitalWrite(pinLEDEyeRight4, HIGH);
	digitalWrite(pinLEDEyeRight5, HIGH);
	digitalWrite(pinLEDEyeRight6, HIGH);
	digitalWrite(pinLEDEyeRight7, HIGH);
	digitalWrite(pinLEDEyeRight8, HIGH);
	digitalWrite(pinLEDEyeRight9, HIGH);
	digitalWrite(pinLEDEyeRight10, HIGH);
	digitalWrite(pinLEDEyeRight11, HIGH);
	digitalWrite(pinLEDEyeRight12, HIGH);

	digitalWrite(pinLEDEyeLeft0, HIGH);
	digitalWrite(pinLEDEyeLeft1, HIGH);
	digitalWrite(pinLEDEyeLeft2, HIGH);
	digitalWrite(pinLEDEyeLeft3, HIGH);
	digitalWrite(pinLEDEyeLeft4, HIGH);
	digitalWrite(pinLEDEyeLeft5, HIGH);
	digitalWrite(pinLEDEyeLeft6, HIGH);
	digitalWrite(pinLEDEyeLeft7, HIGH);
	digitalWrite(pinLEDEyeLeft8, HIGH);
	digitalWrite(pinLEDEyeLeft9, HIGH);
	digitalWrite(pinLEDEyeLeft10, HIGH);
	digitalWrite(pinLEDEyeLeft11, HIGH);
	digitalWrite(pinLEDEyeLeft12, HIGH);
}
void eyesClose(){
	digitalWrite(pinLEDEyeRight0, LOW);
	digitalWrite(pinLEDEyeRight1, LOW);
	digitalWrite(pinLEDEyeRight2, LOW);
	digitalWrite(pinLEDEyeRight3, LOW);
	digitalWrite(pinLEDEyeRight4, LOW);
	digitalWrite(pinLEDEyeRight5, LOW);
	digitalWrite(pinLEDEyeRight6, LOW);
	digitalWrite(pinLEDEyeRight7, LOW);
	digitalWrite(pinLEDEyeRight8, LOW);
	digitalWrite(pinLEDEyeRight9, LOW);
	digitalWrite(pinLEDEyeRight10, LOW);
	digitalWrite(pinLEDEyeRight11, LOW);
	digitalWrite(pinLEDEyeRight12, LOW);

	digitalWrite(pinLEDEyeLeft0, LOW);
	digitalWrite(pinLEDEyeLeft1, LOW);
	digitalWrite(pinLEDEyeLeft2, LOW);
	digitalWrite(pinLEDEyeLeft3, LOW);
	digitalWrite(pinLEDEyeLeft4, LOW);
	digitalWrite(pinLEDEyeLeft5, LOW);
	digitalWrite(pinLEDEyeLeft6, LOW);
	digitalWrite(pinLEDEyeLeft7, LOW);
	digitalWrite(pinLEDEyeLeft8, LOW);
	digitalWrite(pinLEDEyeLeft9, LOW);
	digitalWrite(pinLEDEyeLeft10, LOW);
	digitalWrite(pinLEDEyeLeft11, LOW);
	digitalWrite(pinLEDEyeLeft12, LOW);
}

//Photo Sensor Array
void CDSSensorRead(){
	CDSSensor1FrountValue = analogRead(pinAnalogCDSSensor1Frount);
	CDSSensor2BackValue = analogRead(pinAnalogCDSSensor2Back);
	CDSSensor3LeftValue = analogRead(pinAnalogCDSSensor3Left);
	CDSSensor4RightValue = analogRead(pinAnalogCDSSensor4Right);
	CDSSensor5TopValue = analogRead(pinAnalogCDSSensor5Top);
}

//LCD output
void automationStateDisplay(int displayType){
	if(UptimeMillis - serialPrintLastUpdateMilli > serialPrintRefreshRateMilli) {
		serialPrintLastUpdateMilli = UptimeMillis; 
		if (displayType == displayVerbose) {
			LCDdisplay(automationStateVerboseFormat());
			//LCDdisplayOverride();
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
	switch (LCDPageNumber){
	case 1:
		lcd.setCursor(0, 0);
		lcd.print("State Information");

		lcd.setCursor(0, 1);
		lcd.print(state);
		lcd.setCursor(0, 2);
		lcd.print("MS/S ");
		lcd.print(automationCurrentStateElapsedMillis);
		lcd.print("/");
		lcd.print(automationCurrentStateElapsedMillis / 1000);
		break;

	case 2:
		lcd.setCursor(0,0);
		lcd.print("Motor Information");

		lcd.setCursor(0,1);
		lcd.print("PWM L/R ");
		lcd.print(motorLeftCurentPWM);
		lcd.print("/");
		lcd.print(motorRightCurentPWM);

		lcd.setCursor(0,2);
		lcd.print("Vlt H/C/L ");
		lcd.print((int)motorBatteryVoltageHigh);
		lcd.print("/");
		lcd.print(motorBatteryVoltageNow);
		lcd.print("/");
		lcd.print((int)motorBatteryVoltageLow);

		lcd.setCursor(0,3);
		lcd.print("Hall L/R ");
		lcd.print(wheelHallSensorLeft);
		lcd.print("/");
		lcd.print(wheelHallSensorRight);
		break;

	case 3:
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
		break;

	default://Last page	
		lcd.setCursor(0,0);
		lcd.print("RC Controller");

		lcd.setCursor(0,1);
		lcd.print("PMM X/Y ");
		lcd.print(RCcontrolXPWM);
		lcd.print("/");
		lcd.print(RCcontrolYPWM);

		lcd.setCursor(0,2);
		lcd.print("Y H/L ");
		lcd.print(RCcontrolYPWMTop);
		lcd.print("/");
		lcd.print(RCcontrolYPWMBottom);

		lcd.setCursor(0,3);
		lcd.print("X H/L ");
		lcd.print(RCcontrolXPWMTop);
		lcd.print("/");
		lcd.print(RCcontrolXPWMBottom);

		LCDPageNumber = 0; 
		break;
	}
	LCDPageNumber++;
}

//Read RC controller
void RCReadControlPWM()
{
	RCcontrolXPWM = pulseIn(pinRCcontrollerX, HIGH, 20000);
	if (RCcontrolXPWM != 0){
		RCcontrolXAutoAdjustLimits(RCcontrolXPWM);
		RCcontrolXPWM = RCcontrolXPWM == 0 ? RCcontrolXPWMMiddle : RCcontrolXPWM;
		RCcontrolXPWM = constrain(RCcontrolXPWM, RCcontrolXPWMBottom, RCcontrolXPWMTop);
	}

	RCcontrolYPWM = pulseIn(pinRCcontrollerY, HIGH, 20000);
	if (RCcontrolYPWM != 0){
		RCcontrolYAutoAdjustLimits(RCcontrolYPWM);
		RCcontrolYPWM = RCcontrolYPWM == 0 ? RCcontrolYPWMMiddle : RCcontrolYPWM;
		RCcontrolYPWM = constrain(RCcontrolYPWM, RCcontrolYPWMBottom, RCcontrolYPWMTop);
	}
}
void RCAutomationStateSet(){
	if ((RCcontrolYPWM > 0) && (RCcontrolXPWM > 0))
	{
		RCActivateState();
	}
	else
	{
		actionStopMovment();
		automationStateSet(stateNoRCSignal);
	}
}
void RCcontrolXAutoAdjustLimits(int pinRCcontrollerX){
	RCcontrolXPWMTopDynamic = max(RCcontrolXPWM, RCcontrolXPWMTopDynamic);
	RCcontrolXPWMBottomDynamic = min(RCcontrolXPWM, RCcontrolXPWMBottomDynamic);

	if (RCcontrolXPWMTop < RCcontrolXPWMTopDynamic){
	}
	if (RCcontrolXPWMBottom > RCcontrolXPWMBottomDynamic){
	}

	RCcontrolXPWMRangeSpread = RCcontrolXPWMTop - RCcontrolXPWMBottom;
	RCcontrolXPWMMiddleDynamic = (int)(RCcontrolXPWMRangeSpread * .5) + RCcontrolXPWMBottom;
}
void RCcontrolYAutoAdjustLimits(int pinRCcontrollerY){
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
		int mtrTurnPWM = abs(map(RCcontrolXPWM, RCcontrolXPWMMiddle, RCcontrolXPWMTop, 0, 255)); 
		actionMoveRotateLeft(mtrTurnPWM);
	}
	else if(RCcontrolIsMoveRotateRightRequested()){
		int mtrTurnPWM = abs(map(RCcontrolXPWM, RCcontrolXPWMMiddle, RCcontrolXPWMBottom, 0, 255));
		actionMoveRotateRight(mtrTurnPWM);
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

	digitalWrite(pinHBridgeLeftForward, LOW);
	digitalWrite(pinHBridgeLeftBackward, HIGH);
	digitalWrite(pinHBridgeRightForward, LOW);
	digitalWrite(pinHBridgeRightBackward, HIGH);

	analogWrite(pinHBridgeLeftPWM, motorLeftCurentPWM);
	analogWrite(pinHBridgeRightPWM, motorRightCurentPWM);
}
void actionMoveBackward()
{
	automationStateSet(stateMoveBackward);
	/*Why negative numbers? need abs()*/
	int motorPWM = abs(map(RCcontrolXPWM, RCcontrolXPWMMiddle, RCcontrolXPWMTop, 0, 255));
	motorPWM = constrain(motorPWM, motorMinimumPWMforActuation, 255);
	actionMoveSteer(motorPWM, stateMoveForward);

	digitalWrite(pinHBridgeLeftForward, HIGH);
	digitalWrite(pinHBridgeLeftBackward, LOW);
	digitalWrite(pinHBridgeRightForward, HIGH);
	digitalWrite(pinHBridgeRightBackward, LOW);

	analogWrite(pinHBridgeLeftPWM, motorLeftCurentPWM);
	analogWrite(pinHBridgeRightPWM, motorRightCurentPWM);
}
void actionMoveRotateLeft(int motorPWM){
	automationStateSet(stateMoveRotateLeft);

	digitalWrite(pinHBridgeLeftForward, HIGH);
	digitalWrite(pinHBridgeLeftBackward, LOW);
	digitalWrite(pinHBridgeRightForward, LOW);
	digitalWrite(pinHBridgeRightBackward, HIGH);

	analogWrite(pinHBridgeLeftPWM, motorPWM);
	analogWrite(pinHBridgeRightPWM, motorPWM);
}
void actionMoveRotateLeftWheelForward(int motorPWM){
	digitalWrite(pinHBridgeLeftForward, HIGH);
	digitalWrite(pinHBridgeLeftBackward, LOW);
	digitalWrite(pinHBridgeRightForward, LOW);
	digitalWrite(pinHBridgeRightBackward, LOW);

	analogWrite(pinHBridgeLeftPWM, motorPWM);
	analogWrite(pinHBridgeRightPWM, motorPWM);
}
void actionMoveRotateRight(int motorPWM){
	automationStateSet(stateMoveRotateRight);

	digitalWrite(pinHBridgeLeftForward, LOW);
	digitalWrite(pinHBridgeLeftBackward, HIGH);
	digitalWrite(pinHBridgeRightForward, HIGH);
	digitalWrite(pinHBridgeRightBackward, LOW);

	analogWrite(pinHBridgeLeftPWM, motorPWM);
	analogWrite(pinHBridgeRightPWM, motorPWM);
}
void actionMoveRotateRightWheelForward(int motorPWM){
	digitalWrite(pinHBridgeLeftForward, LOW);
	digitalWrite(pinHBridgeLeftBackward, LOW);
	digitalWrite(pinHBridgeRightForward, HIGH);
	digitalWrite(pinHBridgeRightBackward, LOW);

	analogWrite(pinHBridgeLeftPWM, motorPWM);
	analogWrite(pinHBridgeRightPWM, motorPWM);
}
void actionMoveRotateRightWheelBackward(int motorPWM){
	digitalWrite(pinHBridgeLeftForward, LOW);
	digitalWrite(pinHBridgeLeftBackward, LOW);
	digitalWrite(pinHBridgeRightForward, LOW);
	digitalWrite(pinHBridgeRightBackward, HIGH);

	analogWrite(pinHBridgeLeftPWM, motorPWM);
	analogWrite(pinHBridgeRightPWM, motorPWM);
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
	actionStopMovment();
}
void actionStopMovment(){
	motorLeftCurentPWM = 0;
	motorRightCurentPWM = 0;
	digitalWrite(pinHBridgeLeftBackward, LOW);
	digitalWrite(pinHBridgeLeftForward, LOW);
	digitalWrite(pinHBridgeRightForward, LOW);
	digitalWrite(pinHBridgeRightBackward, LOW);
}

//Additional sensors
void motorBatteryAnalogVoltageDividerRead()
{
	float sensorValue = analogRead(pinAnalogVoltageDivider);
	motorBatteryVoltageNow = (sensorValue / 4.092) * .1;
	motorBatteryVoltageHigh = max(motorBatteryVoltageNow, motorBatteryVoltageHigh);
	motorBatteryVoltageLow = min(motorBatteryVoltageNow, motorBatteryVoltageLow);
}
void wheelHallSensorsRead()
{
	wheelHallSensorLeft = digitalRead(pinHallSensorLeftWheel);
	wheelHallSensorRight = digitalRead(pinHallSensorRightWheel);
}