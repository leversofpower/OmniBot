/* Notes
ch1
rest 1524 - 1532 / Varience 8
high 1876 -1985 direction down / Varience 9
low 1053 - 1065 direction up / Varience 12 

ch2 rest 1502 - 1509 / Varience 7
high 1968 - 1972 direction right / Varience 4
low 1056 - 1062 direction left / Varience 6
Average Varience 7.6

TODO:
Adjust PWM values for more finite control

State Based LCD Display:
when moving just display MOTOR PWM
Create LCD Message que
*/

//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
/*LCD Display */
//LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27
int LCDPageNumber = 0;

/* Pin Assignments */
const int RCcontrolY = 9;
const int RCcontrolX = 8;

const int motorLeftForward = 3;
const int motorLeftBackward = 4;
const int motorLeftPWM = 5; //I think this is wired backwards...

const int motorRightForward = 7;
const int motorRightBackward = 6; 
const int motorRightPWM = 2; //I think this is wired backwards...

const int AnalogVoltageDividerPin = 0;
const int wheelLefttHallSensorPin = 7;
const int wheelRightHallSensorPin = 6;

unsigned long UptimeMillis;

/* Serial Display */
const int serialPrintRefreshRateMilli = 1000;
unsigned long serialPrintLastUpdateMilli = 0;

int RCcontrolYPWM = 0;
int RCcontrolYPWMTop = 1985;
int RCcontrolYPWMMiddle = 1540;
int RCcontrolYPWMBottom = 1061;

//Not fully implemented yet
int RCcontrolYPWMRangeSpread = 0;
int RCcontrolYPWMTopDynamic = RCcontrolYPWMBottom;
int RCcontrolYPWMMiddleDynamic = 0;
int RCcontrolYPWMBottomDynamic = RCcontrolYPWMTop;

int RCcontrolXPWM = 0;
int RCcontrolXPWMTop = 1972;
int RCcontrolXPWMMiddle = 1510;
int RCcontrolXPWMBottom = 1059; 

//Not fully implemented yet
int RCcontrolXPWMRangeSpread = 0;
int RCcontrolXPWMTopDynamic = RCcontrolXPWMBottom;
int RCcontrolXPWMMiddleDynamic = 0;
int RCcontrolXPWMBottomDynamic = RCcontrolXPWMTop;

int RCcontrolPWMYBuffer = 20;
int RCcontrolPWMXBuffer = 20;

int motorLeftCurentPWM = 0;
int motorRightCurentPWM = 0;
int motorMinimumPWMforActuation = 95;

float motorBatteryVoltageNow = 0.0;
float motorBatteryVoltageHigh = 0.0;
float motorBatteryVoltageLow = 12.0;

/* Wheel Hall Sensors */
int wheelHallSensorLeft = 0;
int wheelHallSensorRight = 0;
int wheelRotationsLeft = 0;
int WheelRotationsRight = 0;

/* Enumerations */
const int displayVerbose = 1;

// States 
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

int automationState = stateUndefined;

void setup() {
	Serial.begin(9600);
	//lcd.init();
	//lcd.backlight();

	pinMode(RCcontrolY, INPUT);
	pinMode(RCcontrolX, INPUT);

	pinMode(motorLeftPWM, OUTPUT); 
	pinMode(motorLeftBackward, OUTPUT); 
	pinMode(motorLeftForward, OUTPUT); 

	pinMode(motorRightPWM, OUTPUT); 
	pinMode(motorRightBackward, OUTPUT); 
	pinMode(motorRightForward, OUTPUT);

	pinMode(wheelLefttHallSensorPin, INPUT);
	pinMode(wheelRightHallSensorPin, INPUT);

}
void loop() {
	UptimeMillis = millis();
	//motorBatteryAnalogVoltageDividerRead();
	//wheelHallSensorsRead();

	RCReadControls();

	if (RCsignal()){
		//lcd.setBacklight(1);
		RCActivateState();
	}
	if (! RCsignal()){
		actionStationary();
		automationStateSet(stateNoRCSignal);
	}

	automationStateSerialPrint(displayVerbose);
}

void motorBatteryAnalogVoltageDividerRead()
{
	float sensorValue;
	sensorValue = analogRead(AnalogVoltageDividerPin);
	motorBatteryVoltageNow = (sensorValue / 4.092) * .1;
	motorBatteryVoltageHigh = max(motorBatteryVoltageNow, motorBatteryVoltageHigh);
	motorBatteryVoltageLow = min(motorBatteryVoltageNow, motorBatteryVoltageLow);
}
void wheelHallSensorsRead()
{
	wheelHallSensorLeft = digitalRead(wheelLefttHallSensorPin);
	wheelHallSensorRight = digitalRead(wheelRightHallSensorPin);
}

void automationStateSet(int stateType){
	automationState = stateType;
}
void automationStateSerialPrint(int displayType){
	if(UptimeMillis - serialPrintLastUpdateMilli > serialPrintRefreshRateMilli) {
		serialPrintLastUpdateMilli = UptimeMillis; 
		if (displayType == displayVerbose) {
			Serial.println(automationStateVerboseFormat());
			//LCDdisplay(automationStateVerboseFormat());
		}
	}
}
String automationStateVerboseFormat(){
	switch (automationState){
	case stateUndefined:
		return("Undefined");
	case stateNoRCSignal:
		return("No RC Signal");
	case stateStationary:
		return("Stationary");
	case stateMoveForward:
		return("Forward");
	case stateMoveForwardSteerLeft:
		return("Forward Left");
	case stateMoveForwardSteerRight:
		return("Forward Right");
	case stateMoveBackward:
		return("Reverse");
	case stateMoveBackwardSteerLeft:
		return("Reverse Left");
	case stateMoveBackwardSteerRight:
		return("Reverse Right");
	case stateMoveRotateLeft:
		return("Rotate Left");
	case stateMoveRotateRight:
		return("Rotate Right");
	}
}

void LCDdisplay(String state){
	//lcd.clear();

	if (automationState == stateMoveForward || automationState == stateMoveForwardSteerLeft || automationState == stateMoveForwardSteerRight){
		//lcd.setCursor(0,0);
		//lcd.print("MotorPWM ");
		//lcd.print(motorLeftCurentPWM);
		//lcd.print("/");
		//lcd.print(motorRightCurentPWM);

		//lcd.setCursor(0,1);
		//lcd.print("Vlt ");
		//lcd.print((int)motorBatteryVoltageHigh);
		//lcd.print("/");
		//lcd.print(motorBatteryVoltageNow);
		//lcd.print("/");
		//lcd.print((int)motorBatteryVoltageLow);
	}
	else
	{
		//switch (LCDPageNumber){
		//case 1:
		//	lcd.setCursor(0, 0);
		//	lcd.print(state);
		//	break;

		//case 2:
		//	lcd.setCursor(0,0);
		//	lcd.print("MotorPWM ");
		//	lcd.print(motorLeftCurentPWM);
		//	lcd.print("/");
		//	lcd.print(motorRightCurentPWM);

		//	lcd.setCursor(0,1);
		//	lcd.print("Vlt ");
		//	lcd.print((int)motorBatteryVoltageHigh);
		//	lcd.print("/");
		//	lcd.print(motorBatteryVoltageNow);
		//	lcd.print("/");
		//	lcd.print((int)motorBatteryVoltageLow);
		//	break;

		//case 3:
		//	lcd.setCursor(0,0);
		//	lcd.print("L/R Hall ");
		//	lcd.print(wheelHallSensorLeft);
		//	lcd.print("/");
		//	lcd.print(wheelHallSensorRight);
		//	break;

		//case 4:	
		//	lcd.setCursor(0,0);
		//	lcd.print("RC PMM Y/X ");
		//	lcd.setCursor(0,1);
		//	lcd.print(RCcontrolYPWM);
		//	lcd.print("/");
		//	lcd.print(RCcontrolXPWM);
		//	break;

		//default: //Last page
		//	lcd.setCursor(0,0);
		//	lcd.print("Y ");
		//	lcd.print(RCcontrolYPWMTop);
		//	lcd.print("/");
		//	lcd.print(RCcontrolYPWM);
		//	lcd.print("/");
		//	lcd.print(RCcontrolYPWMBottom);

		//	lcd.setCursor(0,1);
		//	lcd.print("X ");
		//	lcd.print(RCcontrolXPWMTop);
		//	lcd.print("/");
		//	lcd.print(RCcontrolXPWM);
		//	lcd.print("/");
		//	lcd.print(RCcontrolXPWMBottom);

		//	LCDPageNumber = 0; 
		//	break;
		//}
		//LCDPageNumber++;
	}
}

boolean RCsignal()
{
	return RCcontrolYPWM > 0 && RCcontrolXPWM > 0;
}
void RCReadControls()
{
	RCcontrolXPWM = pulseIn(RCcontrolX, HIGH, 25000);
	if (RCcontrolXPWM != 0){
		RCcontrolXAutoAdjustLimits(RCcontrolXPWM);
		RCcontrolXPWM = RCcontrolXPWM == 0 ? RCcontrolXPWMMiddle : RCcontrolXPWM;
		RCcontrolXPWM = constrain(RCcontrolXPWM, RCcontrolXPWMBottom, RCcontrolXPWMTop);
	}

	RCcontrolYPWM = pulseIn(RCcontrolY, HIGH, 25000);
	if (RCcontrolYPWM != 0){
		RCcontrolYAutoAdjustLimits(RCcontrolYPWM);
		RCcontrolYPWM = RCcontrolYPWM == 0 ? RCcontrolYPWMMiddle : RCcontrolYPWM;
		RCcontrolYPWM = constrain(RCcontrolYPWM, RCcontrolYPWMBottom, RCcontrolYPWMTop);
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

// 
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

// Movement PWM Settings and Actuation 
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