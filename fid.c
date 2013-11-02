#include <stdint.h>
#include <stdlib.h>

#include "pd9p.h"

uint32_t
pd9p_newfid(pd9p_session *s) {
	uint32_t fid;
	struct pd9p_fidlinklist *newp;
	if(!(*s).freefids)
		fid=(*s).fidcounter++;
	else {
		fid=(*(*s).freefids).fid;
		newp=(*(*s).freefids).next;
		free((*s).freefids);
		(*s).freefids=newp;
	}
	return fid;
}
