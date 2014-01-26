class ButtonMomentaryNoLed
{
private:
	void (*pInterruptFunctionPress)();
public:
	ButtonMomentaryNoLed(int pin, void (*pInterruptFuncton)());

	unsigned int interruptPin;
	bool isOn;
	bool isOff;
	unsigned long lastPressed;
	unsigned long lastDePressed;
	unsigned long lastTotalPressedTime;
};