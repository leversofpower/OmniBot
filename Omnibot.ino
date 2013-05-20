const int ledPinRight7 = 26;
const int ledPinRight4 = 27;
const int ledPinRight6 = 28;
const int ledPinRight3 = 29;
const int ledPinRight5 = 30;
const int ledPinRight2 = 31;
const int ledPinRight0 = 32;
const int ledPinRight10 = 33; 
const int ledPinRight12 = 34;
const int ledPinRight9 = 35;
const int ledPinRight11 = 36;
const int ledPinRight8 = 37;
const int ledPinRight1 = 38; 

const int ledPinLeft0 = 40;
const int ledPinLeft10 = 41;
const int ledPinLeft12 = 42;
const int ledPinLeft9 = 43;
const int ledPinLeft11 = 44; 
const int ledPinLeft8 = 45; 
const int ledPinLeft7 = 46; 
const int ledPinLeft4 = 47; 
const int ledPinLeft6 = 48; 
const int ledPinLeft3 = 49; 
const int ledPinLeft5 = 50; 
const int ledPinLeft2 = 51; 
const int ledPinLeft1 = 52;

int ledState = LOW;             // ledState used to set the LED
int ledStateInverse = HIGH;
unsigned long currentMillis = 0;
long previousMillis = 0;
long previousMillisLeft = 0;        // will store last time LED was updated
long previousMillisRight = 0;  

long interval = 30;           // interval at which to blink (milliseconds)
long blinkInterval = 5000;

void setup() {
	pinMode(ledPinRight0, OUTPUT); 
	pinMode(ledPinRight1, OUTPUT); 
	pinMode(ledPinRight2, OUTPUT); 
	pinMode(ledPinRight3, OUTPUT); 
	pinMode(ledPinRight4, OUTPUT); 
	pinMode(ledPinRight5, OUTPUT); 
	pinMode(ledPinRight6, OUTPUT); 
	pinMode(ledPinRight7, OUTPUT); 
	pinMode(ledPinRight8, OUTPUT); 
	pinMode(ledPinRight9, OUTPUT); 
	pinMode(ledPinRight10, OUTPUT);  
	pinMode(ledPinRight11, OUTPUT); 
	pinMode(ledPinRight12, OUTPUT);

	pinMode(ledPinLeft0, OUTPUT); 
	pinMode(ledPinLeft1, OUTPUT); 
	pinMode(ledPinLeft2, OUTPUT); 
	pinMode(ledPinLeft3, OUTPUT); 
	pinMode(ledPinLeft4, OUTPUT); 
	pinMode(ledPinLeft5, OUTPUT); 
	pinMode(ledPinLeft6, OUTPUT); 
	pinMode(ledPinLeft7, OUTPUT); 
	pinMode(ledPinLeft8, OUTPUT); 
	pinMode(ledPinLeft9, OUTPUT); 
	pinMode(ledPinLeft10, OUTPUT); 
	pinMode(ledPinLeft11, OUTPUT); 
	pinMode(ledPinLeft12, OUTPUT);
}

void loop()
{
	// here is where you'd put code that needs to be running all the time.

	// check to see if it's time to blink the LED; that is, if the 
	// difference between the current time and last time you blinked 
	// the LED is bigger than the interval at which you want to 
	// blink the LED.
	currentMillis = millis();
	eyesOpen();
	blinkEyeRight();
	blinkEyeLeft();
}
void testLeftEyeLedPostions()
{
	digitalWrite(ledPinLeft0, ledStateInverse);
	digitalWrite(ledPinLeft1, ledStateInverse);
	digitalWrite(ledPinLeft2, ledStateInverse);
	digitalWrite(ledPinLeft3, ledStateInverse);
	digitalWrite(ledPinLeft4, ledStateInverse);
	digitalWrite(ledPinLeft5, ledStateInverse);
	digitalWrite(ledPinLeft6, ledStateInverse);
	digitalWrite(ledPinLeft7, ledStateInverse);
	digitalWrite(ledPinLeft8, ledStateInverse);
	digitalWrite(ledPinLeft9, ledStateInverse);
	digitalWrite(ledPinLeft10, ledStateInverse);
	digitalWrite(ledPinLeft11, ledStateInverse);
	digitalWrite(ledPinLeft12, ledStateInverse);
}
void testRighEyeLedPositions()
{
	digitalWrite(ledPinRight0, HIGH);
	digitalWrite(ledPinRight1, HIGH);
	digitalWrite(ledPinRight2, HIGH);
	digitalWrite(ledPinRight3, HIGH);
	digitalWrite(ledPinRight4, HIGH);
	digitalWrite(ledPinRight5, HIGH);
	digitalWrite(ledPinRight6, HIGH);
	digitalWrite(ledPinRight7, HIGH);
	digitalWrite(ledPinRight8, HIGH);
	digitalWrite(ledPinRight9, HIGH);
	digitalWrite(ledPinRight10, HIGH);
	digitalWrite(ledPinRight11, HIGH);
	digitalWrite(ledPinRight12, HIGH);
}
void flashingEyes()
{

	if(currentMillis - previousMillis > interval) {
		// save the last time you blinked the LED 
		previousMillis = currentMillis;   

		// if the LED is off turn it on and vice-versa:
		if (ledState == LOW)
		{
			ledState = HIGH;
			ledStateInverse = LOW;
		}
		else
		{
			ledState = LOW;
			ledStateInverse = HIGH;
		}
		// set the LED with the ledState of the variable:
		digitalWrite(ledPinRight0, ledState);
		digitalWrite(ledPinRight1, ledState);
		digitalWrite(ledPinRight2, ledState);
		digitalWrite(ledPinRight3, ledState);
		digitalWrite(ledPinRight4, ledState);
		digitalWrite(ledPinRight5, ledState);
		digitalWrite(ledPinRight6, ledState);
		digitalWrite(ledPinRight7, ledState);
		digitalWrite(ledPinRight8, ledState);
		digitalWrite(ledPinRight9, ledState);
		digitalWrite(ledPinRight10, ledState);
		digitalWrite(ledPinRight11, ledState);
		digitalWrite(ledPinRight12, ledState);

		digitalWrite(ledPinLeft0, ledStateInverse);
		digitalWrite(ledPinLeft1, ledStateInverse);
		digitalWrite(ledPinLeft2, ledStateInverse);
		digitalWrite(ledPinLeft3, ledStateInverse);
		digitalWrite(ledPinLeft4, ledStateInverse);
		digitalWrite(ledPinLeft5, ledStateInverse);
		digitalWrite(ledPinLeft6, ledStateInverse);
		digitalWrite(ledPinLeft7, ledStateInverse);
		digitalWrite(ledPinLeft8, ledStateInverse);
		digitalWrite(ledPinLeft9, ledStateInverse);
		digitalWrite(ledPinLeft10, ledStateInverse);
		digitalWrite(ledPinLeft11, ledStateInverse);
		digitalWrite(ledPinLeft12, ledStateInverse);

	}
}
void blinkEyeRight(){
	if(currentMillis - previousMillisRight > blinkInterval) {
		// save the last time you blinked the LED 
		previousMillisRight = currentMillis;   
		digitalWrite(ledPinRight0, LOW);
		digitalWrite(ledPinRight1, LOW);
		delay(30);
		digitalWrite(ledPinRight12, LOW);
		delay(30);
		digitalWrite(ledPinRight2, LOW);
		digitalWrite(ledPinRight11, LOW);
		delay(30);
		digitalWrite(ledPinRight3, LOW);
		digitalWrite(ledPinRight10, LOW);
		delay(30);
		digitalWrite(ledPinRight4, LOW);
		digitalWrite(ledPinRight9, LOW);
		delay(20);
		digitalWrite(ledPinRight5, LOW);
		digitalWrite(ledPinRight8, LOW);
		delay(20);
		digitalWrite(ledPinRight6, LOW);
		digitalWrite(ledPinRight7, LOW);
		//Lid Up
		digitalWrite(ledPinRight5, LOW);
		digitalWrite(ledPinRight8, LOW);
		delay(20);
		digitalWrite(ledPinRight4, LOW);
		digitalWrite(ledPinRight9, LOW);
		delay(20);
		digitalWrite(ledPinRight3, LOW);
		digitalWrite(ledPinRight10, LOW);
		delay(20);
		digitalWrite(ledPinRight2, LOW);
		digitalWrite(ledPinRight11, LOW);
		delay(20);
		digitalWrite(ledPinRight12, LOW);
		delay(20);
		digitalWrite(ledPinRight0, LOW);
		digitalWrite(ledPinRight1, LOW);

	}
}
void blinkEyeLeft(){
	if(currentMillis - previousMillisLeft > blinkInterval) {
		// save the last time you blinked the LED 
		previousMillisLeft = currentMillis;   
		digitalWrite(ledPinLeft0, LOW);
		digitalWrite(ledPinLeft1, LOW);
		delay(30);
		digitalWrite(ledPinLeft12, LOW);
		delay(30);
		digitalWrite(ledPinLeft2, LOW);
		digitalWrite(ledPinLeft11, LOW);
		delay(30);
		digitalWrite(ledPinLeft3, LOW);
		digitalWrite(ledPinLeft10, LOW);
		delay(30);
		digitalWrite(ledPinLeft4, LOW);
		digitalWrite(ledPinLeft9, LOW);
		delay(20);
		digitalWrite(ledPinLeft5, LOW);
		digitalWrite(ledPinLeft8, LOW);
		delay(20);
		digitalWrite(ledPinLeft6, LOW);
		digitalWrite(ledPinLeft7, LOW);
		//Lid Up
		digitalWrite(ledPinLeft5, LOW);
		digitalWrite(ledPinLeft8, LOW);
		delay(20);
		digitalWrite(ledPinLeft4, LOW);
		digitalWrite(ledPinLeft9, LOW);
		delay(20);
		digitalWrite(ledPinLeft3, LOW);
		digitalWrite(ledPinLeft10, LOW);
		delay(20);
		digitalWrite(ledPinLeft2, LOW);
		digitalWrite(ledPinLeft11, LOW);
		delay(20);
		digitalWrite(ledPinLeft12, LOW);
		delay(20);
		digitalWrite(ledPinLeft0, LOW);
		digitalWrite(ledPinLeft1, LOW);
	}
}
void eyesOpen()
{
	digitalWrite(ledPinRight0, HIGH);
	digitalWrite(ledPinRight1, HIGH);
	digitalWrite(ledPinRight2, HIGH);
	digitalWrite(ledPinRight3, HIGH);
	digitalWrite(ledPinRight4, HIGH);
	digitalWrite(ledPinRight5, HIGH);
	digitalWrite(ledPinRight6, HIGH);
	digitalWrite(ledPinRight7, HIGH);
	digitalWrite(ledPinRight8, HIGH);
	digitalWrite(ledPinRight9, HIGH);
	digitalWrite(ledPinRight10, HIGH);
	digitalWrite(ledPinRight11, HIGH);
	digitalWrite(ledPinRight12, HIGH);

	digitalWrite(ledPinLeft0, HIGH);
	digitalWrite(ledPinLeft1, HIGH);
	digitalWrite(ledPinLeft2, HIGH);
	digitalWrite(ledPinLeft3, HIGH);
	digitalWrite(ledPinLeft4, HIGH);
	digitalWrite(ledPinLeft5, HIGH);
	digitalWrite(ledPinLeft6, HIGH);
	digitalWrite(ledPinLeft7, HIGH);
	digitalWrite(ledPinLeft8, HIGH);
	digitalWrite(ledPinLeft9, HIGH);
	digitalWrite(ledPinLeft10, HIGH);
	digitalWrite(ledPinLeft11, HIGH);
	digitalWrite(ledPinLeft12, HIGH);
}
void eyesClose()
{
	digitalWrite(ledPinRight0, LOW);
	digitalWrite(ledPinRight1, LOW);
	digitalWrite(ledPinRight2, LOW);
	digitalWrite(ledPinRight3, LOW);
	digitalWrite(ledPinRight4, LOW);
	digitalWrite(ledPinRight5, LOW);
	digitalWrite(ledPinRight6, LOW);
	digitalWrite(ledPinRight7, LOW);
	digitalWrite(ledPinRight8, LOW);
	digitalWrite(ledPinRight9, LOW);
	digitalWrite(ledPinRight10, LOW);
	digitalWrite(ledPinRight11, LOW);
	digitalWrite(ledPinRight12, LOW);

	digitalWrite(ledPinLeft0, LOW);
	digitalWrite(ledPinLeft1, LOW);
	digitalWrite(ledPinLeft2, LOW);
	digitalWrite(ledPinLeft3, LOW);
	digitalWrite(ledPinLeft4, LOW);
	digitalWrite(ledPinLeft5, LOW);
	digitalWrite(ledPinLeft6, LOW);
	digitalWrite(ledPinLeft7, LOW);
	digitalWrite(ledPinLeft8, LOW);
	digitalWrite(ledPinLeft9, LOW);
	digitalWrite(ledPinLeft10, LOW);
	digitalWrite(ledPinLeft11, LOW);
	digitalWrite(ledPinLeft12, LOW);
}