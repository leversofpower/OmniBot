class AnalogVoltageDivider
{
private:
	struct voltageDivider
	{
		unsigned long time[11];
		int value[11];
	};
	int updateInterval;
	int updateIntervalCycleIndex;
	unsigned long lastUpdateTime;
public:
	void init();
	void update();
	voltageDivider Data[1];
};