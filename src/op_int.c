#include "op_int.h"

void set2Int32(Integer* src, int n) {
  memset(src, 0, g_size_integer);
  src->length = 1;
  if (n < 0) {
    src->sign = 1;
    n = -n;
  }
  src->data[0] = (uint)n;
}

void addUint32(Integer* left, uint right) {
  ulong sum = (ulong)left->data[0];
  sum += (ulong)right;

  left->data[0] = (uint)sum;

  if (sum > 0xffffffffu) {
    int i = 1;
    for (; left->data[i] == 0xffffffffu; ++i) left->data[i] += 1u;
    left->data[i] += 1u;
    if (left->length == i) left->length += 1;
  }
}

void subUint32(Integer* left, uint right) {
  if (left->data[0] >= right) {
    left->data[0] -= right;
  } else if (left->length == 1) {
    left->data[0] = 0u;
  } else {
    ulong sum = 0;
    sum = 0x100000000u + (ulong)left->data[0];
    left->data[0] = (uint)(sum - (ulong)right);
    int i = 1;
    for (; left->data[i] == 0; ++i) left->data[i] = 0xffffffffu;
    left->data[i] -= 1u;
    if (left->data[i] == 0) left->length -= 1;
  }
}

void mulUint32(Integer* left, uint right) {
  ulong sum = 0, carry = 0;
  for (int i = 0; i < left->length; ++i) {
    sum = (ulong)left->data[i] * (ulong)right + carry;
    left->data[i] = (uint)sum;
    carry = sum >> 32;
  }

  if (carry) {
    left->data[left->length] = (uint)carry;
    left->length += 1;
  }
}

void divUint32(Integer* left, uint right) {
  if (left->length == 1) left->data[0] /= right;

  ulong l = 0, r = 0;
  for (int i = left->length - 1; i >= 0; --i) {
    l = r;
    l = (l << 32) + (ulong)left->data[i];
    left->data[i] = (uint)(l / (ulong)right);
    r = l - (ulong)left->data[i] * right;
  }

  if (left->data[left->length-1] == 0) left->length -= 1;
}