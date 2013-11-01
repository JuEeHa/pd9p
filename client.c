#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "pd9p.h"

uint32_t pd9p_msize=1<<13;

uint32_t
pd9p_connect(int fd) {
	char versiondata[12], *p, rcmd, rdata[8192], *rversion, *attachdata, *uname;
	uint16_t rtag, rversionlen, unamelen;
	uint32_t rdatalen, rmsize, fid;
	
	
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
	
	if(strcmp(rversion, "9P2000")) {
		free(rversion);
		return -1;
	}
	free(rversion);
	
	
	if((uname=getenv("LOGNAME")) == 0)
		uname="";
	unamelen=strlen(uname);
	attachdata=malloc(4+4+2+unamelen+2);
	fid=pd9p_newfid();
	p=pd9p_enc4(attachdata, fid);
	p=pd9p_enc4(p, nofid);
	p=pd9p_enc2(p, unamelen);
	memcpy(p, uname, unamelen);
	p+=unamelen;
	pd9p_enc2(p, 0);
	
	if(pd9p_send(fd, Tattach, 0, 4+4+2+unamelen+2, attachdata) == -1) {
		free(attachdata);
		return -1;
	}
	free(attachdata);
	
	
	if(pd9p_recv(fd, &rcmd, &rtag, &rdatalen, rdata) == -1)
		return -1;
		
	if(rcmd!=Rattach)
		return -1;
	if(rdatalen!=13)
		return -1;
	
	return fid;
}
