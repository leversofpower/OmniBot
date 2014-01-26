#include "BluetoothController.h"

BluetoothController::BluetoothController(){

}

void BluetoothController::readSerial(byte serialCommand){
	clearState();
	switch (serialCommand) {
	case 48:
		//Serial.println("IDLE");
		isIdle = true;
		break;
	case 49:
		//Serial.println("UP");
		isPressingUp = true;
		break;
	case 50:
		//Serial.println("DOWN");
		isPressingDown = true;
		break;
	case 51:
		//Serial.println("LEFT");
		isPressingLeft = true;
		break;
	case 52:
		//Serial.println("RIGHT");
		isPressingRight = true;
		break;
	case 53:
		//Serial.println("X");
		isPressingX = true;
		break;
	case 54:
		//Serial.println("CIRCLE");
		isPressingCircle = true;
		break;
	case 55:
		//Serial.println("SQUARE");
		isPressingSquare = true;
		break;
	case 56:
		//Serial.println("TRI");
		isPressingTriangle = true;
		break;
	case 57:
		//Serial.println("SELECT");
		isPressingSelect = true;
		break;
	case 65:
		//Serial.println("START");
		isPressingStart = true;
		break;
	}
}

void BluetoothController::clearState(){
	isIdle = false;
	isPressingUp = false;
	isPressingDown = false;
	isPressingRight = false;
	isPressingLeft = false;
	isPressingSelect = false;
	isPressingStart = false;
	isPressingCircle = false;
	isPressingSquare = false;
	isPressingTriangle = false;
	isPressingX = false;
}
