class ButtonMomentaryNoLed
{
private:
	void (*pInterruptFunctionPress)();
public:
	unsigned int interruptPin;
	bool isOn;
	bool isOff;
	unsigned long lastPressed;
	unsigned long lastDePressed;
	unsigned long lastTotalPressedTime;

	ButtonMomentaryNoLed(int pin, void (*pInterruptFuncton)());
};