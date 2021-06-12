#include "operation.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

const size_t g_size_uint = sizeof(uint);

void lShift(Integer* src, int n) {
  if (n <= 0) return;
  src->length += n;
  memcpy(src->data+n, src->data, g_size_uint * src->length);
  memset(src->data, 0, g_size_uint * n);
  /* for (int i = src->length-1; i >= 0; --i) src->data[i+n] = src->data[i];
     for (int i = 0; i < n; ++i) src->data[i] = 0u; */
}

void rShift(Integer* src, int n) {
  if (n <= 0) return;
  src->length -= n;
  memcpy(src->data, src->data+n, g_size_uint * src->length);
  memset(src->data+src->length-1, 0, g_size_uint * n);
  /* for (int i = 0; i < src->length; ++i) src->data[i] = src->data[i+n];
     for (int i = src->length-1; i >= src->length-n; --i) src->data[i] = 0u; */
}

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

Integer* mulDiv(const Integer* left, const Integer* right, int op) {
  Integer* dst = NULL;

  if (op) {
    int cmp_value = cmp_(left, right);
    if (cmp_value == -1) return initInteger();
    else if (cmp_value == 0) return toInteger(1);
    else if (isOne_(right) == 0) return copyInteger(left);

    dst = initInteger();
    div_(left, right, dst);
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

Integer* mod(const Integer* left, const Integer* right) {
  Integer* dst = NULL;

  int cmp_value = cmp_(left, right);
  if (cmp_value == -1) return copyInteger(left);
  else if (cmp_value == 0) return initInteger();

  dst = copyInteger(left);
  // mod_(left, right, dst);

  dst->sign = left->sign;
  return dst;
}


/*
 *    PRIVATE FUNCTION
 */

int cmp_(const Integer* left, const Integer* right) {
  if (left->length > right->length) return 1;
  else if (left->length < right->length) return -1;
  /* left->length == right->length */
  for (int i = left->length-1; i >= 0; --i)
    if (left->data[i] == right->data[i]) continue;
    else if (left->data[i] > right->data[i]) return 1;
    else /*if (left->data[i] < right->data[i])*/ return -1;
  /* left->data[...] ==  right->data[...] */
  return 0;
}

int isZero_(const Integer* src) {
  if (src->length > 1) return 1;
  if (src->data[0] != 0) return 1;
  return 0;
}

int isOne_(const Integer* src) {
  if (src->length > 1) return 1;
  if (src->data[0] != 1) return 1;
  return 0;
}

void add_(const Integer* left, const Integer* right, Integer* dst) {
  Integer* new_left = copyInteger(left), *new_right = copyInteger(right);
  dst->length = left->length;

  ulong carry = 0u, sum = 0u;
  for (int i = 0; i < new_left->length; ++i) {
    sum = (ulong)new_left->data[i]
        + (ulong)new_right->data[i]
        + carry;
    dst->data[i] = (uint)sum;
    carry = sum >> 32;
  }

  dst->data[dst->length] = (uint)carry;
  dst->length += (int)carry;

  free(new_left);
  free(new_right);
}

void sub_(const Integer* left, const Integer* right, Integer* dst) {
  Integer* new_left = copyInteger(left), *new_right = copyInteger(right);
  dst->length = left->length;

  ulong carry = 0u, sum = 0u;
  for (int i = 0; i < new_left->length; ++i) {
    if (
      (new_left->data[i] > new_right->data[i]) || 
      ((new_left->data[i] == new_right->data[i]) && (carry == 0))
    ) {
      sum = (ulong)new_left->data[i]
          - (ulong)new_right->data[i]
          - carry;
      carry = 0u;
    } else {
      sum = 0x100000000u + (ulong)new_left->data[i]
          - (ulong)new_right->data[i]
          - carry;
      carry = 1u;
    }
    dst->data[i] = (uint)sum;
  }

  while (dst->data[dst->length-1] == 0 && dst->length > 1) dst->length -= 1;

  free(new_left);
  free(new_right);
}

void mul_(const Integer* left, const Integer* right, Integer* dst) {
  Integer* new_left = copyInteger(left), *new_right = copyInteger(right);
  memset(dst->data, 0, g_size_uint * INT_LEN);
  dst->length = left->length + right->length - 1;

  ulong carry = 0u, sum = 0u;
  for (int i = 0; i < new_left->length; ++i) {
    carry = 0u;
    for (int j = 0; j < new_right->length; ++j) {
      sum = (ulong)new_left->data[i]
          * (ulong)new_right->data[j]
          + (ulong)dst->data[i+j]
          + carry;
      dst->data[i+j] = (uint)sum;
      carry = sum >> 32;
    }
    dst->data[new_right->length + i] = (uint)carry;
  }

  if (carry) {
    dst->data[dst->length] = (uint)carry;
    dst->length += 1;
  }

  free(new_left);
  free(new_right);
}

void div_(const Integer* left, const Integer* right, Integer* dst) {
  Integer* new_left = copyInteger(left), *new_right = copyInteger(right);
  
  int len = 0;
  ulong l = 0, r = (ulong)new_right->data[new_right->length-1];
  while (cmp_(new_left, new_right) >= 0) {
    l = (ulong)new_left->data[new_left->length-1];
    len = new_left->length - new_right->length;

    if (l == r && len == 0) {
      Integer* one = toInteger(1);
      add_(dst, one, dst);
      free(one);
      break;
    } else if (l <= r /* && len */) {
      len -= 1;
      l = (l << 32) + (ulong)new_left->data[new_left->length-2];
    }

    l /= (r+1);
    Integer* tmp_right = l2Integer((long)l);
    if (len) lShift(tmp_right, len);

    if (dst->length >= tmp_right->length) add_(dst, tmp_right, dst);
    else add_(tmp_right, dst, dst);

    mul_(new_right, tmp_right, tmp_right);

    if (cmp_(new_left, tmp_right) <= 0) {
      free(tmp_right);
      break;
    } else {
      sub_(new_left, tmp_right, new_left);
    }
    free(tmp_right);
  }
  
  free(new_left);
  free(new_right);
}
