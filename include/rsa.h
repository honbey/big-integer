#ifndef RSA_H_
#define RSA_H_

#include "core.h"

#define VERBOSE 0x00000001
#define DECIMAL 0x00000002

Integer **initRSAParams(int bits, int args);

typedef uint8_t uchar;

uchar *RSA(const uchar *message, int *length, const Integer *key,
           const Integer *n);

uchar *char2Uchar(const char *message, int length);

#endif
