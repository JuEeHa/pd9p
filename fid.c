#include <stdint.h>
#include <stdlib.h>

void **pd9p_unused_fids=0;
uint32_t pd9p_fid_counter=0;

uint32_t
pd9p_newfid(void) {
	uint32_t fid;
	void **newp;
	if(!pd9p_unused_fids)
		fid=pd9p_fid_counter++;
	else {
		fid=*(uint32_t*)(pd9p_unused_fids[0]);
		newp=(void**)pd9p_unused_fids[1];
		free(pd9p_unused_fids[0]);
		free(pd9p_unused_fids);
		pd9p_unused_fids=newp;
	}
	return fid;
}
