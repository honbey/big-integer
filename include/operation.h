#ifndef OPERATION_H_
#define OPERATION_H_

#include "core.h"

Integer *plusMinus(const Integer *left, const Integer *right, int sign);
Integer *mulDiv(const Integer *left, const Integer *right, int op,
                Integer *rem);

/* if use function `doX()`, please make sure `dst != left or right` */
void doPlusMinus(const Integer *left, const Integer *right, int sign,
                 Integer *dst);
void doMulDiv(const Integer *left, const Integer *right, int op, Integer *dst);

Integer *modExponent(const Integer *src, const Integer *exp,
                     const Integer *mod);

Integer *modInversion(const Integer *src, const Integer *mod);
Integer *gcdEuclid(const Integer *left, const Integer *right);

int primalityFermat(const Integer *src);

static void lShift(Integer *src, int n);
static void rShift(Integer *src, int n);

static inline void lShift(Integer *src, int n) {
  if (n <= 0)
    return;
  src->length += n;
  memcpy(src->data + n, src->data, g_size_uint * src->length);
  memset(src->data, 0, g_size_uint * n);
  /* for (int i = src->length-1; i >= 0; --i) src->data[i+n] = src->data[i];
     for (int i = 0; i < n; ++i) src->data[i] = 0u; */
}

static inline void rShift(Integer *src, int n) {
  if (n <= 0)
    return;
  src->length -= n;
  memcpy(src->data, src->data + n, g_size_uint * src->length);
  memset(src->data + src->length - 1, 0, g_size_uint * n);
  /* for (int i = 0; i < src->length; ++i) src->data[i] = src->data[i+n];
     for (int i = src->length-1; i >= src->length-n; --i) src->data[i] = 0u; */
}

#endif
