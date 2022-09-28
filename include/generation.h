#ifndef GENERATION_H_
#define GENERATION_H_

#include "core.h"

Integer *generateInteger(int bits);
Integer *str2Integer(const char *src);
Integer *generatePrimeInteger(int bits);

void printInteger(const Integer *src, int base);
uint toBase(const Integer *left, uint right, Integer *dst);

Integer *initInteger();
Integer *toInteger(int n);
Integer *l2Integer(long n);

#endif
