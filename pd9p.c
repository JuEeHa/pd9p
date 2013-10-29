#include "pd9p.h"

int
main(void) {
	if(pd9p_connect(1))
		return 1;
	return 0;
}
