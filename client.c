#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "pd9p.h"

uint32_t pd9p_msize=1<<13;

int
pd9p_connect(int fd) {
	char versiondata[12], *p, rcmd, rdata[pd9p_msize], *rversion;
	uint16_t rtag, rversionlen;
	uint32_t rdatalen, rmsize;
	
	
	p=pd9p_enc4(versiondata, pd9p_msize);
	p=pd9p_enc2(p, 6);
	memcpy(p, "9P2000", 6);
	
	if(pd9p_send(fd, Tversion, notag, 12, versiondata) == -1)
		return -1;
	
		
	if(pd9p_recv(fd, &rcmd, &rtag, &rdatalen, rdata) == -1)
		return -1;
	if(rcmd!=Rversion)
		return -1;
	if(rdatalen<6)
		return -1;
	
	p=pd9p_dec4(rdata, &rmsize);
	if(rmsize<pd9p_msize)
		pd9p_msize=rmsize;
	
	p=pd9p_dec2(p, &rversionlen);
	if(rdatalen < (uint16_t)(6+rversionlen))
		return -1;
	
	rversion=malloc(rversionlen+1);
	memcpy(rversion, p, rversionlen);
	rversion[rversionlen]=0;
	
	if(strcmp(rversion, "9P2000"))
		return -1;
	
	
	return 0;
}
