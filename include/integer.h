#ifndef INTEGER_H_
#define INTEGER_H_

#define MAX_BIT      4096
#define BIN_EXP_BASE 32
#define INT_LEN      128

// typedef unsigned char uchar;
typedef unsigned int  uint;
typedef unsigned long ulong;

typedef struct {
  int sign;
  int length;
  int prime;
  int reserve;
  uint data[INT_LEN];
  /* uint *data; */
} Integer;


void printInteger(const Integer* src, int base);



uint toStr_(const Integer* left, uint right, Integer* dst);

#include "generation.h"
#include "operation.h"

#endif