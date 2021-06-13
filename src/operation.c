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
  Integer* dst = NULL;

  if (op) {
    int cmp_value = cmp_(left, right);
    if (rem /* != NULL */) {
      if (cmp_value == -1) {
        rem = copyInteger(left);
        return initInteger();
      }
      else if (cmp_value == 0) {
        rem = initInteger();
        return toInteger(1);
      }
      else if (isOne_(right) == 0) {
        rem = initInteger();
        return copyInteger(left);
      }
    } else {
      if (cmp_value == -1) return initInteger();
      else if (cmp_value == 0) return toInteger(1);
      else if (isOne_(right) == 0) return copyInteger(left);
    }

    dst = initInteger();
    div_(left, right, dst, rem);
  } else {
    if (isZero_(left) == 0 || isZero_(right) == 0) return initInteger();
    else if (isOne_(left) == 0) return copyInteger(right);
    else if (isOne_(right) == 0) return copyInteger(left);

    dst = initInteger();
    mul_(left, right, dst);
  }

  dst->sign = left->sign ^ right->sign;
  return dst;
}
