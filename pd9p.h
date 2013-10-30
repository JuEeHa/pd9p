#ifndef __PD9P_H
#define __PD9P_H

#include <stdint.h>

#define Tversion 100
#define Rversion 101

#define notag 0xFFFF

extern uint32_t pd9p_msize;

/* encdec.c */

char *pd9p_enc1(char *buf, uint8_t val); /* buf+1 */
char *pd9p_enc2(char *buf, uint16_t val); /* buf+2 */
char *pd9p_enc4(char *buf, uint32_t val); /* buf+4 */

char *pd9p_dec1(char *buf, uint8_t *val); /* buf+1 */
char *pd9p_dec2(char *buf, uint16_t *val); /* buf+2 */
char *pd9p_dec4(char *buf, uint32_t *val); /* buf+4 */

/* comm.c */

int pd9p_send(int fd, char cmd, uint16_t tag, uint32_t datalen, char *data); /* amount of data written */
int pd9p_recv(int fd, char *cmd, uint16_t *tag, uint32_t *datalen, char *data); /* amount of data read */

/* client.c */
int pd9p_connect(int fd); /* 0 on success, -1 on error */

#endif
