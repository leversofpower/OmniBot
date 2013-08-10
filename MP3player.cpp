#include "MP3player.h"
#include <Arduino.h>

void MP3player::init()
{

}
void MP3player::playTrack(int trackID){
	/** select SD card first music and play */
	cmd_buf[0] = 0x7E;          // START
	cmd_buf[1] = 0x04;          // Length
	cmd_buf[2] = 0xA2;          // Command
	cmd_buf[3] = 0x00;          // file number high byte
	cmd_buf[4] = trackID;       // file number low byte
	cmd_buf[5] = 0x7E;          // END
	SendCMD(cmd_buf, 6);
}
void MP3player::volume_high(){
	cmd_buf[0] = 0x7E;          // START
	cmd_buf[1] = 0x03;          // Length
	cmd_buf[2] = 0xA7;          // Command
	cmd_buf[3] = 0x1F;          // new volume
	cmd_buf[4] = 0x7E;          // END
	SendCMD(cmd_buf, 5);
}
void MP3player::single_play(){
	cmd_buf[0] = 0x7E;          // START
	cmd_buf[1] = 0x03;          // Length
	cmd_buf[2] = 0xA9;          // Command SET MODE
	cmd_buf[3] = 0x00;          // set mode
	cmd_buf[4] = 0x7E;          // END
	SendCMD(cmd_buf, 5);
}
void MP3player::volume_medium(){
	cmd_buf[0] = 0x7E;          // START
	cmd_buf[1] = 0x03;          // Length
	cmd_buf[2] = 0xA7;          // Command
	cmd_buf[3] = 0x0F;          // new volume
	cmd_buf[4] = 0x7E;          // END
	SendCMD(cmd_buf, 5);
}
void MP3player::volume_low(){
	cmd_buf[0] = 0x7E;          // START
	cmd_buf[1] = 0x03;          // Length
	cmd_buf[2] = 0xA7;          // Command
	cmd_buf[3] = 0x05;          // new volume
	cmd_buf[4] = 0x7E;          // END
	SendCMD(cmd_buf, 5);
}
void MP3player::SendCMD(unsigned char *cmd_buf, unsigned int len) {
	unsigned i;
	for (i = 0; i < len; i++) {
		Serial1.write(cmd_buf[i]);
	}
}