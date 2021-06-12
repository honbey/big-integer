#include "generation.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const size_t g_size_integer = sizeof(Integer);

inline Integer* initInteger() {
  Integer* dst = (Integer* )malloc(g_size_integer);
  memset(dst, 0, g_size_integer);
  dst->length  = 1;
  return dst;
}

inline Integer* copyInteger(const Integer* src) {
  Integer* dst = (Integer* )malloc(g_size_integer);
  memcpy(dst, src, g_size_integer);
  return dst;
}

inline Integer* toInteger(int n) {
  Integer* dst = (Integer* )malloc(g_size_integer);
  memset(dst, 0, g_size_integer);
  if (n < 0) {
    n = -n;
    dst->sign = 1;
  }
  dst->length = 1;
  dst->data[0] = (uint)(n);
  return dst;
}

inline Integer* l2Integer(long n) {
  Integer* dst = (Integer* )malloc(g_size_integer);
  memset(dst, 0, g_size_integer);
  if (n < 0) {
    n = -n;
    dst->sign = 1;
  }
  dst->length = 1;
  dst->data[0] = (uint)(n);
  if (n > 0xffffffff) {
    dst->length = 2;
    dst->data[1] = (uint)(n >> 32);
  }
  return dst;
}

Integer* generateInteger(int bits) {
  Integer* dst = (Integer* )malloc(g_size_integer);
  memset(dst, 0, g_size_integer);
  dst->length = 1;

  for (int i = 0; i < bits/BIN_EXP_BASE-1; ++i) {
    dst->length += 1;
    dst->data[i] = rand();
  }

  dst->data[bits/BIN_EXP_BASE-1] = rand() & 0xffffu + 0x10000u;
  dst->data[0] |= 0x1u;
  return dst;
}