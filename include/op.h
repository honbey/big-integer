#ifndef OP_H_
#define OP_H_


#include "core.h"


static void set_(Integer* src, uint n);
static int isZero_(const Integer* src);
static int isOne_(const Integer* src);


static int cmp_(const Integer* left, const Integer* right);
static void add_(const Integer* left, const Integer* right, Integer* dst);
static void sub_(const Integer* left, const Integer* right, Integer* dst);
static void mul_(const Integer* left, const Integer* right, Integer* dst);
static void div_(const Integer* left, const Integer* right, Integer* dst);


static inline void set_(Integer* src, uint n) {
  memset(src, 0, g_size_uint * (INT_LEN+4));
  src->length = 1;
  src->data[0] = n;
}

static inline int isZero_(const Integer* src) {
  if (src->length > 1) return 1;
  if (src->data[0] /* != 0 */) return 1;
  return 0;
}

static inline int isOne_(const Integer* src) {
  if (src->length > 1) return 1;
  if (src->data[0] != 1) return 1;
  return 0;
}


/*
 *    PRIVATE FUNCTION
 */

static int cmp_(const Integer* left, const Integer* right) {
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

static void add_(const Integer* left, const Integer* right, Integer* dst) {
  memcpy(dst, left, g_size_integer);

  ulong carry = 0u, sum = 0u;
  for (int i = 0; i < right->length; ++i) {
    sum = (ulong)left->data[i]
        + (ulong)right->data[i]
        + carry;
    dst->data[i] = (uint)sum;
    carry = sum >> 32;
  }

  dst->data[dst->length] = (uint)carry;
  dst->length += (int)carry;
}

static void sub_(const Integer* left, const Integer* right, Integer* dst) {
  memcpy(dst, left, g_size_integer);
  
  ulong carry = 0u, sum = 0u;
  for (int i = 0; i <= right->length; ++i) {
    if (
      (left->data[i]  >  right->data[i]) || 
      ((left->data[i] == right->data[i]) && (carry == 0))
    ) {
      sum = (ulong)left->data[i]
          - (ulong)right->data[i]
          - carry;
      carry = 0u;
    } else {
      sum = 0x100000000u + (ulong)left->data[i]
          - (ulong)right->data[i]
          - carry;
      carry = 1u;
    }
    dst->data[i] = (uint)sum;
  }

  while (dst->data[dst->length-1] == 0 && dst->length > 1) dst->length -= 1;
}

static void mul_(const Integer* left, const Integer* right, Integer* dst) {
  dst->length = left->length + right->length - 1;

  ulong carry = 0u, sum = 0u;
  for (int i = 0; i < left->length; ++i) {
    carry = 0u;
    for (int j = 0; j < right->length; ++j) {
      sum = (ulong)left->data[i]
          * (ulong)right->data[j]
          + (ulong)dst->data[i+j]
          + carry;
      dst->data[i+j] = (uint)sum;
      carry = sum >> 32;
    }
    dst->data[right->length + i] = (uint)carry;
  }

  if (carry) {
    dst->data[dst->length] = (uint)carry;
    dst->length += 1;
  }
}

static void div_(const Integer* left, const Integer* right, Integer* dst) {
  Integer* new_left  = copyInteger(left);
  Integer* new_right = copyInteger(right);
  
  int len = 0;
  ulong l = 0, r = (ulong)new_right->data[new_right->length-1];
  while (cmp_(new_left, new_right) >= 0) {
    l = (ulong)new_left->data[new_left->length-1];
    len = new_left->length - new_right->length;

    Integer* tmp = initInteger();
    if (l == r && len == 0) {
      sub_(tmp, new_right, new_left);
      set_(new_right, 1u);
      add_(tmp, new_right, dst);
      free(tmp);
      break;
    } else if (l <= r /* && len */) {
      len -= 1;
      l = (l << 32) + (ulong)new_left->data[new_left->length-2];
    }

    l /= (r+1);
    Integer* tmp_right = l2Integer((long)l);
    if (len) lShift(tmp_right, len);

    if (dst->length < tmp_right->length) add_(tmp_right, dst, tmp);
    else add_(dst, tmp_right, tmp);
    memcpy(dst, tmp, g_size_integer);
    
 /* because division use multiplication, it's necessary 
  * to prevent the disturbution of _dst_'s original data.*/
    set_(tmp, 0u);
    mul_(new_right, tmp_right, tmp);
    memcpy(tmp_right, tmp, g_size_integer);

    if (cmp_(new_left, tmp_right) >= 0) sub_(new_left, tmp_right, new_left);
    
    free(tmp);
    free(tmp_right);
  }
  printInteger(new_left, 10); // it's remainder
  free(new_left);
  free(new_right);
}


#endif