#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

#include "pd9p.h"

int
sockconnect(char *name, uint16_t port) {
	int s;
	struct hostent *he;
	struct sockaddr_in addr;
	
	if((he=gethostbyname(name)) == NULL)
		return -1;
	
	memcpy(&addr.sin_addr, he->h_addr, he->h_length);
	addr.sin_port=htons(port);
	addr.sin_family=AF_INET;
	
	if((s=socket(AF_INET, SOCK_STREAM, 0)) == -1)
		return -1;
	if(connect(s, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1) {
		close(s);
		return -1;
	}
	
	return s;
}

int
main(int argc, char **argv) {
	int s;
	pd9p_session *session;
	uint32_t file;
	
	if(argc<4) {
		fputs("Usage: pd9p ADDR PORT PATH\n", stderr);
		return 1;
	}
	
	if((s=sockconnect(argv[1], atoi(argv[2]))) == -1) {
		fputs("pd9p: error: sockconnect\n", stderr);
	}
	
	if((session=pd9p_connect(s)) == 0) {
		close(s);
		fputs("pd9p: error: pd9p_connec\n", stderr);
		return 1;
	}
	
	printf("open(%s): %u\n", argv[3], file=pd9p_open(session, argv[3], pd9p_rdonly));
	if(file!=errfid)
		printf("close(%u): %i\n", file, pd9p_close(session, file));
	
	pd9p_closesession(session);
	close(s);
	return 0;
}
