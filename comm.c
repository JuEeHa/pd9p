#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "pd9p.h"

int
pd9p_send(int fd, char cmd, uint32_t tag, uint32_t datalen, char *data) {
	char *buf, *p;
	uint32_t sent;
	uint32_t size;
	
	size=4+1+2+datalen;
	if(size>pd9p_msize)
		return -1;
	if((buf=malloc(size)) == 0)
		exit(1);
	p=pd9p_enc4(buf, size);
	p=pd9p_enc1(p, cmd);
	p=pd9p_enc2(p, tag);
	memcpy(p, data, datalen);
	
	for(sent=0; sent<size;)
		sent+=write(fd, buf+sent, size-sent);
	return 0;
}
