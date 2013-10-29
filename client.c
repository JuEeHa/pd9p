#include <string.h>

#include "pd9p.h"

int
pd9p_connect(int fd) {
	char versiondata[12], *p;
	
	p=pd9p_enc4(versiondata, pd9p_msize);
	p=pd9p_enc2(p, 6);
	memcpy(p, "9P2000", 6);
	
	if(pd9p_send(1, Tversion, notag, 12, versiondata))
		return 1;
}
