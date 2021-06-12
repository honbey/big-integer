#ifndef OPERATION_H_
#define OPERATION_H_

#include "integer.h"

void lShift(Integer* src, int n);
void rShift(Integer* src, int n);
Integer* plusMinus(const Integer* left, const Integer* right, int sign);
Integer* mulDiv(const Integer* left, const Integer* right, int op);
Integer* mod(const Integer* left, const Integer* right);



int cmp_(const Integer* left, const Integer* right);
int isZero_(const Integer* src);
int isOne_(const Integer* src);
void add_(const Integer* left, const Integer* right, Integer* dst);
void sub_(const Integer* left, const Integer* right, Integer* dst);
void mul_(const Integer* left, const Integer* right, Integer* dst);
void div_(const Integer* left, const Integer* right, Integer* dst);

#endif