#ifndef __PD9P_H
#define __PD9P_H

#include <stdint.h>

#define Tversion 100
#define Rversion 101

#define notag 0xFFFF

char *pd9p_enc1(char *buf, uint8_t val); /* returns buf+1 */
char *pd9p_enc2(char *buf, uint16_t val); /* returns buf+2 */
char *pd9p_enc4(char *buf, uint32_t val); /* returns buf+4 */

#define pd9p_msize (1<<13)

#endif
