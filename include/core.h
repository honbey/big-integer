#ifndef CORE_H_
#define CORE_H_

#define MAX_BIT      4096
#define BIN_EXP_BASE 32
#define INT_LEN      128

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* typedef uint8_t uchar; */
typedef uint32_t uint;
typedef uint64_t ulong;

typedef struct {
  int sign;
  int length;
  int prime;
  int reserve;
  uint data[INT_LEN];
  /* uint *data; */
} Integer;

static const size_t g_size_uint = sizeof(uint);
static const size_t g_size_integer = sizeof(Integer);

#endif