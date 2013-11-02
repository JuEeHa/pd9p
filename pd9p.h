#ifndef __PD9P_H
#define __PD9P_H

#include <stdint.h>

#define Tversion 100
#define Rversion 101
#define Tauth    102
#define Rauth    103
#define Tattach  104
#define Rattach  105

#define notag 0xFFFF
#define nofid 0xFFFFFFFF

struct pd9p_fidlinklist {
	uint32_t fid;
	struct pd9p_fidlinklist *next;
};

struct pd9p_session {
	int fd;
	uint32_t msize;
	uint32_t rootfid;
	struct pd9p_fidlinklist *freefids;
	uint32_t fidcounter;
};

typedef struct pd9p_session pd9p_session;

/* encdec.c */

char *pd9p_enc1(char *buf, uint8_t val); /* buf+1 */
char *pd9p_enc2(char *buf, uint16_t val); /* buf+2 */
char *pd9p_enc4(char *buf, uint32_t val); /* buf+4 */

char *pd9p_dec1(char *buf, uint8_t *val); /* buf+1 */
char *pd9p_dec2(char *buf, uint16_t *val); /* buf+2 */
char *pd9p_dec4(char *buf, uint32_t *val); /* buf+4 */

/* comm.c */

int pd9p_send(pd9p_session *s, char cmd, uint16_t tag, uint32_t datalen, char *data); /* amount of data written */
int pd9p_recv(pd9p_session *s, char *cmd, uint16_t *tag, uint32_t *datalen, char *data); /* amount of data read */

/* client.c */
pd9p_session *pd9p_connect(int fd); /* session on success, 0 on error */
void pd9p_closesession(pd9p_session *s);

/* fid.c */
uint32_t pd9p_newfid(pd9p_session *s); /* unused fid */

#endif
