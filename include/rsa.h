#ifndef RSA_H_
#define RSA_H_

#include "core.h"

#define VERBOSE 0x00000001
#define DECIMAL 0x00000002

Integer** initRSAParams(int bits, int args);

#endif