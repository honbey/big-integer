#ifndef OP_H_
#define OP_H_

#include "core.h"

void set_(Integer *src, uint n);
int isZero_(const Integer *src);
int isOne_(const Integer *src);

/*
 *    PRIVATE FUNCTION
 */
int cmp_(const Integer *left, const Integer *right);
void add_(const Integer *left, const Integer *right, Integer *dst);
void sub_(const Integer *left, const Integer *right, Integer *dst);
void mul_(const Integer *left, const Integer *right, Integer *dst);
void div_(const Integer *left, const Integer *right, Integer *quot,
          Integer *rem);

#endif
