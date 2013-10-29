#include <stdint.h>

char*
pd9p_enc1(char *buf, uint8_t val) {
	buf[0]=val;
	return buf+1;
}

char*
pd9p_enc2(char *buf, uint16_t val) {
	buf[0]=val%(1<<8);
	buf[1]=val>>8;
	return buf+2;
}

char*
pd9p_enc4(char *buf, uint32_t val) {
	buf[0]=val%(1<<8);
	buf[1]=(val>>8)%(1<<8);
	buf[2]=(val>>16)%(1<<8);
	buf[3]=val>>24;
	return buf+4;
}
