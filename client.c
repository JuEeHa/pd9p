#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "pd9p.h"

uint32_t pd9p_msize=1<<13;

pd9p_session*
pd9p_connect(int fd) {
	char versiondata[12], *p, rcmd, rdata[8192], *rversion, *attachdata, *uname;
	uint16_t rtag, rversionlen, unamelen;
	uint32_t rdatalen, rmsize;
	pd9p_session s, *ret;
	
	s.fd=fd;
	s.msize=1<<13;
	s.freefids=0;
	s.fidcounter=0;
	
	p=pd9p_enc4(versiondata, pd9p_msize);
	p=pd9p_enc2(p, 6);
	memcpy(p, "9P2000", 6);
	
	if(pd9p_send(&s, Tversion, notag, 12, versiondata) == -1)
		return 0;
	
	
	if(pd9p_recv(&s, &rcmd, &rtag, &rdatalen, rdata) == -1)
		return 0;
	if(rcmd!=Rversion)
		return 0;
	if(rdatalen<6)
		return 0;
	
	p=pd9p_dec4(rdata, &rmsize);
	if(rmsize<pd9p_msize)
		pd9p_msize=rmsize;
	
	p=pd9p_dec2(p, &rversionlen);
	if(rdatalen < (uint16_t)(6+rversionlen))
		return 0;
	
	rversion=malloc(rversionlen+1);
	memcpy(rversion, p, rversionlen);
	rversion[rversionlen]=0;
	
	if(strcmp(rversion, "9P2000")) {
		free(rversion);
		return 0;
	}
	free(rversion);
	
	
	if((uname=getenv("LOGNAME")) == 0)
		uname="";
	unamelen=strlen(uname);
	attachdata=malloc(4+4+2+unamelen+2);
	s.rootfid=pd9p_newfid(&s);
	p=pd9p_enc4(attachdata, s.rootfid);
	p=pd9p_enc4(p, nofid);
	p=pd9p_enc2(p, unamelen);
	memcpy(p, uname, unamelen);
	p+=unamelen;
	pd9p_enc2(p, 0);
	
	if(pd9p_send(&s, Tattach, 0, 4+4+2+unamelen+2, attachdata) == -1) {
		free(attachdata);
		return 0;
	}
	free(attachdata);
	
	
	if(pd9p_recv(&s, &rcmd, &rtag, &rdatalen, rdata) == -1)
		return 0;
		
	if(rcmd!=Rattach)
		return 0;
	if(rdatalen!=13)
		return 0;
	
	ret=malloc(sizeof(pd9p_session));
	memcpy(ret, &s, sizeof(pd9p_session));
	return ret;
}

void
pd9p_closesession(pd9p_session *s) {
	struct pd9p_fidlinklist *fllp;
	struct pd9p_fidlinklist *nextfllp;
	/* Clean all linked lists */
	for(fllp=(*s).freefids; fllp; fllp=nextfllp) {
		nextfllp=(*fllp).next;
		free(fllp);
	}
	
	free(s);
}
