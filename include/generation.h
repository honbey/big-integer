#ifndef GENERATION_H_
#define GENERATION_H_

#include "integer.h"

Integer* initInteger();

Integer* copyInteger(const Integer* src);
Integer* toInteger(int n);
Integer* l2Integer(long n);

Integer* generateInteger(int bits);

#endif