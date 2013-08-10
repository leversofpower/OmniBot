class MP3player
{
private:
	unsigned char cmd_buf[10];
	unsigned char i;
	void SendCMD(unsigned char *cmd_buf, unsigned int len);
public:
	enum track
	{
		startUp = 1,
		command = 2,
		swoosh = 3
	};
	void init();
	void playTrack(int);
	void single_play();
	void volume_low();
	void volume_medium();
	void volume_high();
};