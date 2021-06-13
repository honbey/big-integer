#include "generation.h"
#include "operation.h"
#include "op.h"


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

    if (cmp_value == 1) {
      sub_(left, right, dst);
      dst->sign = left->sign;
    }

    if (cmp_value == -1) {
      sub_(right, left, dst);
      dst->sign = 1 - left->sign;
    }
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
    int cmp_value = cmp_(left, right);
    if (cmp_value == -1) {
      if (rem) memcpy(rem, left, g_size_integer);
      set_(dst, 0u);
    } else if (cmp_value == 0) {
      if (rem) set_(rem, 0u);
      set_(dst, 1u);
    } else if (isOne_(right) == 0) {
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
