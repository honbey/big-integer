#include "generation.h"
#include "operation.h"
#include "op.h"
#include "op_int.h"


Integer* plusMinus(const Integer* left, const Integer* right, int sign) {
/*
 *  L  R  O  'Real Operation'  'L^O^R'
 *  +  +  +       +               0
 *  +  -  +       -               1
 *  -  +  +       -               1
 *  -  -  +       +               0
 *  +  +  -       -               1
 *  +  -  -       +               0
 *  -  +  -       +               0
 *  -  -  -       -               1
 */
  if (left == NULL || right == NULL) return NULL;

  Integer* dst = initInteger();

  int sign_xor = (left->sign ^ sign) ^ right->sign;
  if (sign_xor) {
    int cmp_value = cmp_(left, right);
    if (cmp_value == 0) return dst;
    else if (cmp_value ==  1) sub_(left, right, dst);
    else if (cmp_value == -1) sub_(right, left, dst);
    dst->sign = cmp_value * (((cmp_value-1) / 2) + left->sign);
  } else {
    if (left->length < right->length) add_(right, left, dst);
    else add_(left, right, dst);
    dst->sign = left->sign;
  }

  return dst;
}

Integer* mulDiv(const Integer* left,
                const Integer* right,
                int op,
                Integer* rem) {
  if (left == NULL || right == NULL) return NULL;

  Integer* dst = initInteger();

  if (op) {
    if (isOne_(right) == 0) {
      if (rem) set_(rem, 0u);
      memcpy(dst, left, g_size_integer);
    } else if (isZero_(right) == 0) {
      printf("\n*Error: The dividend is zero, program will return zero.*\n");
    } else {
      div_(left, right, dst, rem);
    }
  } else {
    if (isZero_(left) == 0 || isZero_(right) == 0) set_(dst, 0u);
    else if (isOne_(left) == 0) memcpy(dst, right, g_size_integer);
    else if (isOne_(right) == 0) memcpy(dst, left, g_size_integer);
    else mul_(left, right, dst);
  }

  dst->sign = left->sign ^ right->sign;
  return dst;
}

void doPlusMinus(const Integer* left,
                const Integer* right,
                int sign,
                Integer* dst) {
  int sign_xor = (left->sign ^ sign) ^ right->sign;
  if (sign_xor) {
    int cmp_value = cmp_(left, right);
    dst->sign = cmp_value * (((cmp_value-1) / 2) + left->sign);
    if (cmp_value == 0) set_(dst, 0u);
    else if (cmp_value ==  1) sub_(left, right, dst);
    else if (cmp_value == -1) sub_(right, left, dst);
  } else {
    if (left->length < right->length) add_(right, left, dst);
    else add_(left, right, dst);
    dst->sign = left->sign;
  }
}

void doMulDiv(const Integer* left,
              const Integer* right,
              int op,
              Integer* dst) {
  memset(dst->data, 0, g_size_uint * INT_LEN);

  if (op) {
    dst->length = 1;
    if (isOne_(right) == 0) memcpy(dst, left, g_size_integer);
    else if (isZero_(right) == 0) 
      printf("\n*Error: The dividend is zero, program will return zero.*\n");
    else div_(left, right, dst, NULL);
  } else {
    if (isZero_(left) == 0 || isZero_(right) == 0) set_(dst, 0u);
    else if (isOne_(left) == 0) memcpy(dst, right, g_size_integer);
    else if (isOne_(right) == 0) memcpy(dst, left, g_size_integer);
    else mul_(left, right, dst);
  }

  dst->sign = left->sign ^ right->sign;
}

Integer* modExponent(const Integer* src,
                     const Integer* exp,
                     const Integer* mod) {
  Integer* dst = initInteger();

  Integer* new_src = (Integer* )malloc(g_size_integer);
  Integer* new_exp = (Integer* )malloc(g_size_integer);
  memcpy(new_exp, exp, g_size_integer);

  div_(src, mod, dst, new_src);
  set_(dst, 1u);

  Integer* two = toInteger(2);
  Integer* tmp = (Integer* )malloc(g_size_integer);
  while (isZero_(new_exp)) {
    if (new_exp->data[0] & 1) {
    /* memcpy(tmp, new_exp, g_size_integer);
       sub_(tmp, one, new_exp); */

      memcpy(tmp, dst, g_size_integer);
      set_(dst, 0u);
      mul_(tmp, new_src, dst);

      set_(tmp, 0u);
      div_(dst, mod, tmp, dst);
    }

    memcpy(tmp, new_exp, g_size_integer);
    set_(new_exp, 0u);
    div_(tmp, two, new_exp, NULL);

    memcpy(tmp, new_src, g_size_integer);
    set_(new_src, 0u);
    mul_(tmp, tmp, new_src);

    set_(tmp, 0u);
    div_(new_src, mod, tmp, new_src);
  }

  free(two);
  free(tmp);
  free(new_src);
  free(new_exp);

  return dst;
}

/* Extended Euclid Algorithm */
Integer* modInversion(const Integer* src, const Integer* mod) {
  Integer* t1 = toInteger(1);
  Integer* t2 = toInteger(0);
  Integer* t3 = (Integer* )malloc(g_size_integer);
  memcpy(t3, src, g_size_integer);

  Integer* r1 = toInteger(0);
  Integer* r2 = toInteger(1);
  Integer* r3 = (Integer* )malloc(g_size_integer);
  memcpy(r3, mod, g_size_integer);

  Integer* tmp1 = (Integer* )malloc(g_size_integer);
  Integer* tmp2 = (Integer* )malloc(g_size_integer);
  Integer* tmp3 = (Integer* )malloc(g_size_integer);

  Integer* quot = initInteger();
  Integer* tmp  = initInteger();

  while (isZero_(r3)) {
    set_(quot, 0u);
    div_(t3, r3, quot, NULL);

    memcpy(tmp1, r1, g_size_integer);
    memcpy(tmp2, r2, g_size_integer);
    memcpy(tmp3, r3, g_size_integer);

    doMulDiv(quot, r1, 0, tmp);
    doPlusMinus(t1, tmp, 1, r1);

    doMulDiv(quot, r2, 0, tmp);
    doPlusMinus(t2, tmp, 1, r2);

    doMulDiv(quot, r3, 0, tmp);
    doPlusMinus(t3, tmp, 1, r3);

    memcpy(t1, tmp1, g_size_integer);
    memcpy(t2, tmp2, g_size_integer);
    memcpy(t3, tmp3, g_size_integer);
  }

  Integer* dst = initInteger();
  div_(t1, mod, dst, dst);

  if (t1->sign) {
    dst->sign = 0;
    memcpy(tmp, dst, g_size_integer);
    sub_(mod, tmp, dst);
  }

  free(t1);
  free(t2);
  free(t3);
  free(r1);
  free(r2);
  free(r3);
  free(tmp1);
  free(tmp2);
  free(tmp3);
  free(tmp);
  free(quot);

  return dst;
}

Integer* gcdEuclid(const Integer* left, const Integer* right) {
  Integer* new_left = (Integer* )malloc(g_size_integer);
  memcpy(new_left, left, g_size_integer);
  Integer* new_right = (Integer* )malloc(g_size_integer);
  memcpy(new_right, right, g_size_integer);

  Integer* tmp = initInteger();

  while (isZero_(new_left)) {
    set_(tmp, 0u);
    div_(new_right, new_left, tmp, tmp);

    memcpy(new_right, new_left, g_size_integer);
    memcpy(new_left, tmp, g_size_integer);
  }

  free(tmp);
  free(new_left);

  return new_right;
}

int primalityFermat(const Integer* src) {
  Integer* tmp = (Integer* )malloc(g_size_integer);
  for (int i = 0; i < 1000; ++i) {
    set_(tmp, prime_table[i]);
    div_(src, tmp, tmp, tmp);
    if (isZero_(tmp) == 0) {
      free(tmp);
      return 1;
    }
  }

  memcpy(tmp, src, g_size_integer);
  subUint32(tmp, 1u);
  for (int i = 0; i < 10; ++i) {
    int len = rand() % (src->length-2) + 1;
    Integer* b = generateInteger(len * BIN_EXP_BASE);
    Integer* r = modExponent(b, tmp, src);
    if (isOne_(r) != 0) { /* r != 1 */
      free(b);
      free(r);
      free(tmp);
      return 1;
    }
    free(b);
    free(r);
  }

  free(tmp);
  return 0;
}
