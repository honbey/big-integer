#ifndef GENERATION_H_
#define GENERATION_H_


#include "core.h"


Integer* generateInteger(int bits);
Integer* str2Integer(const char* src);
Integer* generatePrimeInteger(int bits);


void printInteger(const Integer* src, int base);
uint toBase(const Integer* left, uint right, Integer* dst);


static Integer* initInteger();
static Integer* toInteger(int n);
static Integer* l2Integer(long n);


static inline Integer* initInteger() {
  Integer* dst = (Integer* )malloc(g_size_integer);
  memset(dst, 0, g_size_integer);
  dst->length  = 1;
  return dst;
}

static inline Integer* toInteger(int n) {
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

static inline Integer* l2Integer(long n) {
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

#endif