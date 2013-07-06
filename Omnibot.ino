/*
NEEDS FIXED:
NEEDS MEASURED:
One hall check rotation angle
one hall check on both wheels distance forward and backward
TODO:
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
const int pinHBridgeLeftPWM = 5;
const int pinHBridgeRightForward = 7;
const int pinHBridgeRightBackward = 6; 
const int pinHBridgeRightPWM = 2;
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
const int pinLEDBackPowerButton = 10;
const int pinRelayPlasmaGlobe = 23;
const int pinHallSensorLeftWheel = 40;
const int pinHallSensorRightWheel = 41;
/* Pin Assignments Analog */
const int pinAnalogVoltageDivider = 0;
const int pinAnalogCDSSensor1Frount = 1;
const int pinAnalogCDSSensor2Back = 2;
const int pinAnalogCDSSensor3Left = 3;
const int pinAnalogCDSSensor4Right = 4;
const int pinAnalogCDSSensor5Top = 5;
const int displayVerbose = 1;

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
} stateType;

/* Library Objects */
//LiquidCrystal_I2C lcd(0x27,16,2); OLD LCD
LiquidCrystal_I2C lcd(0x27,20,4);
//Timers 
Timer timerEyeBlink;

/* Globals */
unsigned long UptimeMillis;
const int serialPrintRefreshRateMilli = 2000;
unsigned long serialPrintLastUpdateMilli = 0;
// automation state
int automationCurrentState = 0;
unsigned long automationCurrentStateElapsedMillis = 0;
unsigned long automationStateLastMillisMeasure = 0;
int motorLeftCurentPWM = 0;
int motorRightCurentPWM = 0;
int motorMinimumPWMforActuation = 95; //Is this actually correct?
float motorBatteryVoltageNow = 0.0;
float motorBatteryVoltageHigh = 0.0;
float motorBatteryVoltageLow = 12.0;


// Hall Sensors - ned to make a struct out of this
int wheelHallSensorLeft = 0;
int wheelHallSensorRight = 0;
int wheelRotationsLeft = 0;
int WheelRotationsRight = 0;
int wheelRotationsLeftState = 0;
int wheelRotationsRightState = 0;

int commandStep = 0; // sequencing hall movement commands
int hallRotaionCheckRateMilli = 2000; //millisecond time used to wait for next hall detection
unsigned long hallRotaionLastCheckMilli = 0; // Last check time
int hallMilliPassedCheck = 0; //number of completed checks using hallRotaionCheckRateMilli timer
int hallMovementChecks = 0; //Used to hold automated movement return counts

//LCD Screen
int LCDPageNumber = 0;
// Angel eye power button 
int backPowerButtonPWM = 0;
int backPowerButtonPulseRate = 70;
int backPowerButtonPulseSteps[] = {0,1,2,5,10,255,20,30,40,50,60,70,80,90,100,255,100,90,80,70,60,50,40,30,20,10,0};
int backPowerButtonPulsePosition = 0;
unsigned long backPowerButtonLastPulse = 0;
struct RCcontrollerPWM
{
	int current;
	int maximum;
	int resting;
	int minimum;
	int tolerence;
} RCx,RCy;
struct photoCell
{
	int frount;
	int back;
	int left;
	int right;
	int top;
} photoCellValue;

// Automated Behavior
unsigned long automatedBehaviorLastUpdateMilli = 0;
int automatedBehaviorRefreshRateMilli = 500;


void setup() {
	Serial.begin(9600);

	RCx.current = 0;
	RCx.maximum = 1973;
	RCx.resting = 1540;
	RCx.minimum = 1061;
	RCx.tolerence = 50;

	RCy.current = 0;
	RCy.maximum = 1972;
	RCy.resting = 1510;
	RCy.minimum = 1059;
	RCy.tolerence = 50;

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
}

void loop() {
	UptimeMillis = millis();
	//RC Controller
	RCReadControlPWM();
	RCAutomationStateSet();

	//Eyes
	timerEyeBlink.update();
	//Back Power Button
	backPowerButtonPulseRateChange();
	backPowerButtonPulse();
	//Wheel Rotations
	wheelHallSensorsRead();
	//CDS Monitoring
	CDSSensorRead();

	//FaceTheLight(); //WIP

	//Battery Monitoring
	motorBatteryAnalogVoltageDividerRead();
	//LCD SCreen
	automationStateDisplay(displayVerbose);
}

/* Behaviors */
// Rotate twords light
void FaceTheLight(){ //WIP
	//if(UptimeMillis - automatedBehaviorLastUpdateMilli > automatedBehaviorRefreshRateMilli) {
	//automatedBehaviorLastUpdateMilli = UptimeMillis; 

	//if (CDSSensor3LeftValue > CDSSensor4RightValue){
	//	//Rotate Left
	//	actionMoveLeftRotate(130);
	//}
	//else if (CDSSensor4RightValue > CDSSensor3LeftValue)  {
	//	//Rotate Right
	//	actionMoveRightRotate(130);
	//}
	//else{
	//	actionMoveStop();
	//}

	//store all four values in an array
	int sensor[3];
	sensor[0] = photoCellValue.frount;
	sensor[1] = photoCellValue.back;
	sensor[2] = photoCellValue.left;
	sensor[3] = photoCellValue.right;
	// Read all four values
	// Find the highest value of the four
	int index = getIndexOfMaximumValue(sensor, 4);
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
	case NoRCSignal:
		return("No RC Signal");
	case Stationary:
		return("Stationary X:" + String(RCx.current) + ", Y:" + String(RCy.current));
	case MoveForward:
		return("Forward X:" + String(RCx.current) + ", Y:" + String(RCy.current));
	case MoveForwardSteerLeft:
		return("Forward Left X:" + String(RCx.current) + ", Y:" + String(RCy.current));
	case MoveForwardSteerRight:
		return("Forward Right X:" + String(RCx.current) + ", Y:" + String(RCy.current));
	case MoveBackward:
		return("Reverse X:" + String(RCx.current) + ", Y:" + String(RCy.current));
	case MoveBackwardSteerLeft:
		return("Reverse Left X:" + String(RCx.current) + ", Y:" + String(RCy.current));
	case MoveBackwardSteerRight:
		return("Reverse Right X:" + String(RCx.current) + ", Y:" + String(RCy.current));
	case MoveRotateLeft:
		return("Rotate Left X:" + String(RCx.current) + ", Y:" + String(RCy.current));
	case MoveRotateRight:
		return("Rotate Right X:" + String(RCx.current) + ", Y:" + String(RCy.current));
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
	if ((automationCurrentState == MoveForward) || (automationCurrentState == MoveForwardSteerLeft) || (automationCurrentState == MoveForwardSteerRight)
		|| (automationCurrentState == MoveBackward) || (automationCurrentState == MoveBackwardSteerLeft) || (automationCurrentState == MoveBackwardSteerRight)
		|| (automationCurrentState == MoveRotateLeft) || (automationCurrentState == MoveRotateRight)){
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
	else if ((automationCurrentState == Stationary) ||  (automationCurrentState == NoRCSignal)){
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
	photoCellValue.frount = analogRead(pinAnalogCDSSensor1Frount);
	photoCellValue.back = analogRead(pinAnalogCDSSensor2Back);
	photoCellValue.left = analogRead(pinAnalogCDSSensor3Left);
	photoCellValue.right = analogRead(pinAnalogCDSSensor4Right);
	photoCellValue.top = analogRead(pinAnalogCDSSensor5Top);
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
	lcd.print(photoCellValue.top);

	lcd.setCursor(0,2);
	lcd.print("Frnt ");
	lcd.print(photoCellValue.frount);
	lcd.print(" Back  ");
	lcd.print(photoCellValue.back);

	lcd.setCursor(0,3);
	lcd.print("Left ");
	lcd.print(photoCellValue.left);
	lcd.print(" Right ");
	lcd.print(photoCellValue.right);
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
		lcd.print(photoCellValue.top);

		lcd.setCursor(0,2);
		lcd.print("Frnt ");
		lcd.print(photoCellValue.frount);
		lcd.print(" Back  ");
		lcd.print(photoCellValue.back);

		lcd.setCursor(0,3);
		lcd.print("Left ");
		lcd.print(photoCellValue.left);
		lcd.print(" Right ");
		lcd.print(photoCellValue.right);
		break;

	default://Last page	
		lcd.setCursor(0,0);
		lcd.print("RC Controller");

		lcd.setCursor(0,1);
		lcd.print("PMM X/Y ");
		lcd.print(RCx.current);
		lcd.print("/");
		lcd.print(RCy.current);

		lcd.setCursor(0,2);
		lcd.print("Y H/L ");
		lcd.print(RCy.maximum);
		lcd.print("/");
		lcd.print(RCy.minimum);

		lcd.setCursor(0,3);
		lcd.print("X H/L ");
		lcd.print(RCx.maximum);
		lcd.print("/");
		lcd.print(RCx.minimum);

		LCDPageNumber = 0; 
		break;
	}
	LCDPageNumber++;
}

//Read RC controller
void RCReadControlPWM()
{
	RCx.current = pulseIn(pinRCcontrollerX, HIGH, 20000);
	if (RCx.current != 0){
		RCx.current = RCx.current == 0 ? RCx.resting : RCx.current;
		RCx.current = constrain(RCx.current, RCx.minimum, RCx.maximum);
	}

	RCy.current = pulseIn(pinRCcontrollerY, HIGH, 20000);
	if (RCy.current != 0){
		RCy.current = RCy.current == 0 ? RCy.resting : RCy.current;
		RCy.current = constrain(RCy.current, RCy.minimum, RCy.maximum);
	}
}
void RCAutomationStateSet(){
	if ((RCy.current > 0) && (RCx.current > 0))
	{
		RCActivateState();
		//When RC controller is turned on begin testing simple movement routines
		//hallMovementChecks = 2;

		if ((hallAlignWheels() == true) && (commandStep == 0)){
			Serial.println("Hall Wheels Aligned");
			commandStep = 1;
			delay(5000);
		}

		if (commandStep == 1){
			hallMovementChecks = 2;
			if (hallRotateRight() == true){
				commandStep = 2;
				Serial.println("Command step 2 completed");
				hallMilliPassedCheck = 0;
			}
		}
	}
	else
	{
		commandStep = 0;
		actionMoveStop();
		automationStateSet(NoRCSignal);
	}
}
void RCActivateState(){
	if(RCcontrolIsMoveForwardRequested()) {
		actionMoveForward();
	}
	else if(RCControlIsMoveBackwardRequested()) {
		actionMoveBackward();
	} 
	else if(RCcontrolIsMoveRotateLeftRequested()){
		int mtrTurnPWM = abs(map(RCx.current, RCx.resting, RCx.maximum, 0, 255)); 
		actionMoveLeftRotate(mtrTurnPWM);
	}
	else if(RCcontrolIsMoveRotateRightRequested()){
		int mtrTurnPWM = abs(map(RCx.current, RCx.resting, RCx.minimum, 0, 255));
		actionMoveRightRotate(mtrTurnPWM);
	}
	else{
		actionStationary();
	}
}

// Movement trigger tolerances
boolean RCcontrolIsMoveForwardRequested()
{
	return RCy.current < (RCy.resting - RCy.tolerence);
}
boolean RCControlIsMoveBackwardRequested()
{
	return RCy.current > (RCy.resting  + RCy.tolerence); 
}
boolean RCcontrolIsMoveRotateLeftRequested(){
	return RCx.current < (RCx.resting  - RCx.tolerence);  
}
boolean RCcontrolIsMoveRotateRightRequested(){
	return RCx.current > (RCx.resting  + RCx.tolerence);
}

// Movement PWM settings and actuation 
void actionMoveForward()
{
	automationStateSet(MoveForward);
	/*Why negative numbers? need abs()*/
	int motorPWM = abs(map(RCy.current, RCy.resting , RCy.maximum, 0, 255));
	motorPWM = constrain(motorPWM, motorMinimumPWMforActuation, 255);

	actionMoveSteer(motorPWM, MoveForward);

	digitalWrite(pinHBridgeLeftForward, HIGH);
	digitalWrite(pinHBridgeLeftBackward, LOW);
	digitalWrite(pinHBridgeRightForward, HIGH);
	digitalWrite(pinHBridgeRightBackward, LOW);

	analogWrite(pinHBridgeLeftPWM, motorLeftCurentPWM);
	analogWrite(pinHBridgeRightPWM, motorRightCurentPWM);
}
void actionMoveBackward()
{
	automationStateSet(MoveBackward);
	/*Why negative numbers? need abs()*/
	int motorPWM = abs(map(RCy.current, RCy.resting , RCy.maximum, 0, 255));
	motorPWM = constrain(motorPWM, motorMinimumPWMforActuation, 255);

	actionMoveSteer(motorPWM, MoveBackward);

	digitalWrite(pinHBridgeLeftForward, LOW);
	digitalWrite(pinHBridgeLeftBackward, HIGH);
	digitalWrite(pinHBridgeRightForward, LOW);
	digitalWrite(pinHBridgeRightBackward, HIGH);

	analogWrite(pinHBridgeLeftPWM, motorLeftCurentPWM);
	analogWrite(pinHBridgeRightPWM, motorRightCurentPWM);
}
void actionMoveLeftRotate(int motorPWM){
	automationStateSet(MoveRotateLeft);

	digitalWrite(pinHBridgeLeftForward, LOW);
	digitalWrite(pinHBridgeLeftBackward, HIGH);
	digitalWrite(pinHBridgeRightForward, HIGH);
	digitalWrite(pinHBridgeRightBackward, LOW);

	analogWrite(pinHBridgeLeftPWM, motorPWM);
	analogWrite(pinHBridgeRightPWM, motorPWM);
}
void actionMoveRightRotate(int motorPWM){
	automationStateSet(MoveRotateRight);

	digitalWrite(pinHBridgeLeftForward, HIGH);
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

	if (state == MoveForward)
	{
		if (RCcontrolIsMoveRotateLeftRequested())
		{
			automationStateSet(MoveForwardSteerLeft);
			mtrTurnPWM = abs(map(RCx.current, RCx.resting , RCx.minimum, 0, 255));
			motorRightCurentPWM = constrain(mtrTurnPWM + motorLeftCurentPWM, 0, 255);
			motorLeftCurentPWM = constrain((mtrTurnPWM * -1 ) + motorRightCurentPWM, 0,255);
		}
		else if (RCcontrolIsMoveRotateRightRequested())
		{
			automationStateSet(MoveForwardSteerRight);
			mtrTurnPWM = abs(map(RCx.current, RCx.resting , RCx.maximum, 0, 255)); 
			motorRightCurentPWM = constrain((mtrTurnPWM * -1 ) + motorLeftCurentPWM, 0, 255);
			motorLeftCurentPWM = constrain(mtrTurnPWM + motorRightCurentPWM, 0, 255);
		}
	}
	else if (state = MoveBackward)
	{
		if (RCcontrolIsMoveRotateLeftRequested())
		{
			automationStateSet(MoveBackwardSteerLeft);
			mtrTurnPWM = abs(map(RCx.current, RCx.resting , RCx.minimum, 0, 255));
			motorRightCurentPWM = constrain(mtrTurnPWM + motorLeftCurentPWM, 0, 255);
			motorLeftCurentPWM = constrain((mtrTurnPWM * -1 ) + motorRightCurentPWM, 0, 255);

		}
		else if (RCcontrolIsMoveRotateRightRequested())
		{
			automationStateSet(MoveBackwardSteerRight);
			mtrTurnPWM = abs(map(RCx.current, RCx.resting , RCx.maximum, 0, 255)); 
			motorRightCurentPWM = constrain((mtrTurnPWM * -1 ) + motorLeftCurentPWM, 0, 255);
			motorLeftCurentPWM = constrain(mtrTurnPWM + motorRightCurentPWM, 0, 255);

		}
	}
}
void actionStationary()
{
	automationStateSet(Stationary);
	actionMoveStop();
}
void actionMoveStop(){
	motorLeftCurentPWM = 0;
	motorRightCurentPWM = 0;
	digitalWrite(pinHBridgeLeftBackward, LOW);
	digitalWrite(pinHBridgeLeftForward, LOW);
	digitalWrite(pinHBridgeRightForward, LOW);
	digitalWrite(pinHBridgeRightBackward, LOW);
}
//HAll Movement
bool hallAlignWheels(){
	wheelHallSensorsRead();

	if ((wheelHallSensorLeft == 0) && (wheelHallSensorRight == 0)){
		digitalWrite(pinHBridgeLeftForward, HIGH);
		digitalWrite(pinHBridgeLeftBackward, LOW);
		digitalWrite(pinHBridgeRightForward, LOW);
		digitalWrite(pinHBridgeRightBackward, HIGH);

		analogWrite(pinHBridgeLeftPWM, 150);
		analogWrite(pinHBridgeRightPWM, 150);
	}
	else if ((wheelHallSensorLeft == 1) && (wheelHallSensorRight == 0)){
		digitalWrite(pinHBridgeLeftForward, LOW);
		digitalWrite(pinHBridgeLeftBackward, LOW);
		digitalWrite(pinHBridgeRightForward, LOW);
		digitalWrite(pinHBridgeRightBackward, HIGH);

		analogWrite(pinHBridgeLeftPWM, 150);
		analogWrite(pinHBridgeRightPWM, 150);
	}
	else if ((wheelHallSensorLeft == 0) && (wheelHallSensorRight == 1)){
		digitalWrite(pinHBridgeLeftForward, LOW);
		digitalWrite(pinHBridgeLeftBackward, LOW);
		digitalWrite(pinHBridgeRightForward, LOW);
		digitalWrite(pinHBridgeRightBackward, HIGH);

		analogWrite(pinHBridgeLeftPWM, 0);
		analogWrite(pinHBridgeRightPWM, 150);
	}
	else if ((wheelHallSensorLeft == 1) && (wheelHallSensorRight == 1)){
		digitalWrite(pinHBridgeLeftForward, LOW);
		digitalWrite(pinHBridgeLeftBackward, LOW);
		digitalWrite(pinHBridgeRightForward, LOW);
		digitalWrite(pinHBridgeRightBackward, LOW);

		analogWrite(pinHBridgeLeftPWM, 0);
		analogWrite(pinHBridgeRightPWM, 0);
	}

	if ((wheelHallSensorLeft == 1) && (wheelHallSensorRight == 1))
	{ 
		return true;
	}
	else
	{
		return false;
	}
}
//WIP
bool hallRotateRight(){
	digitalWrite(pinHBridgeLeftForward, LOW);
	digitalWrite(pinHBridgeLeftBackward, HIGH);
	digitalWrite(pinHBridgeRightForward, HIGH);
	digitalWrite(pinHBridgeRightBackward, LOW);

	analogWrite(pinHBridgeLeftPWM, 150);
	analogWrite(pinHBridgeRightPWM, 150);

	//don't check instantly, need time for some rotation
	if(UptimeMillis - hallRotaionLastCheckMilli > hallRotaionCheckRateMilli) {
		hallMilliPassedCheck = hallMilliPassedCheck + 1; //may need to set this to zero higher scope
		hallRotaionLastCheckMilli = UptimeMillis; 
		if (hallMilliPassedCheck > 1)
		{
			Serial.println("Time Passed");
		}
	}

	if (hallMilliPassedCheck > 1){
		if ((wheelHallSensorLeft == 0) && (wheelHallSensorRight == 0)){
			digitalWrite(pinHBridgeLeftForward, LOW);
			digitalWrite(pinHBridgeLeftBackward, HIGH);
			digitalWrite(pinHBridgeRightForward, HIGH);
			digitalWrite(pinHBridgeRightBackward, LOW);

			analogWrite(pinHBridgeLeftPWM, 150);
			analogWrite(pinHBridgeRightPWM, 150);
		}
		else if ((wheelHallSensorLeft == 1) && (wheelHallSensorRight == 0)){
			digitalWrite(pinHBridgeLeftForward, LOW);
			digitalWrite(pinHBridgeLeftBackward, LOW);
			digitalWrite(pinHBridgeRightForward, HIGH);
			digitalWrite(pinHBridgeRightBackward, LOW);

			analogWrite(pinHBridgeLeftPWM, 150);
			analogWrite(pinHBridgeRightPWM, 150);
		}
		else if ((wheelHallSensorLeft == 0) && (wheelHallSensorRight == 1)){
			digitalWrite(pinHBridgeLeftForward, LOW);
			digitalWrite(pinHBridgeLeftBackward, HIGH);
			digitalWrite(pinHBridgeRightForward, LOW);
			digitalWrite(pinHBridgeRightBackward, LOW);

			analogWrite(pinHBridgeLeftPWM, 150);
			analogWrite(pinHBridgeRightPWM, 150);
		}
		else if ((wheelHallSensorLeft == 1) && (wheelHallSensorRight == 1)){
			digitalWrite(pinHBridgeLeftForward, LOW);
			digitalWrite(pinHBridgeLeftBackward, LOW);
			digitalWrite(pinHBridgeRightForward, LOW);
			digitalWrite(pinHBridgeRightBackward, LOW);

			analogWrite(pinHBridgeLeftPWM, 0);
			analogWrite(pinHBridgeRightPWM, 0);
			return true;
		}
		return false;
	}
	else
	{
		return false;
	}
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