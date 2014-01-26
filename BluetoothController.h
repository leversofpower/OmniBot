#include <Arduino.h>

class BluetoothController{
private:
	void clearState();
public:
	BluetoothController();
	void readSerial(byte serialCommand);
	bool hasSignal;
	bool isIdle;
	bool isPressingUp;
	bool isPressingDown;
	bool isPressingRight;
	bool isPressingLeft;
	bool isPressingSelect;
	bool isPressingStart;
	bool isPressingCircle;
	bool isPressingSquare;
	bool isPressingTriangle;
	bool isPressingX;
};