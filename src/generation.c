#include "generation.h"
#include "op_int.h"
#include "operation.h"

Integer *generateInteger(int bits) {
  Integer *dst = (Integer *)malloc(g_size_integer);
  memset(dst, 0, g_size_integer);
  dst->length = 1;

  for (int i = 0; i < bits / BIN_EXP_BASE - 1; ++i) {
    dst->length += 1;
    dst->data[i] = rand();
  }

  dst->data[bits / BIN_EXP_BASE - 1] = rand() & 0xffffu + 0x10000u;
  dst->data[0] |= 0x1u;
  return dst;
}

Integer *str2Integer(const char *src) {
  Integer *bit = toInteger(1);
  Integer *tmp = initInteger();
  Integer *dst = initInteger();

  int len = strlen(src);
  for (int i = len - 1; i >= 0; --i) {
    memcpy(tmp, bit, g_size_integer);
    mulUint32(tmp, (uint)(src[i] - '0'));
    doPlusMinus(dst, tmp, 0, dst);
    mulUint32(bit, 10u);
  }

  free(bit);
  free(tmp);
  return dst;
}

Integer *generatePrimeInteger(int bits) {
  Integer *p = NULL;
  while (1) {
    p = generateInteger(bits);
    if (primalityFermat(p))
      free(p);
    else
      return p;
  }
}

void printInteger(const Integer *src, int base) {
  if (base < 0 || 16 < base)
    base = 16;
  if (src->sign)
    printf("-");
  if (src->length == 1) {
    printf("%u\n", src->data[0]);
  } else {
    Integer *tmp = (Integer *)malloc(g_size_integer);
    memcpy(tmp, src, g_size_integer);

    char char_table[] = "0123456789ABCDEF";
    size_t size = sizeof(char) * (size_t)(src->length * BIN_EXP_BASE * 0.33);
    char *str = (char *)malloc(size);
    memset(str, 0, sizeof(size));
    int i = 0;
    for (; tmp->length > 1 || tmp->data[0] != 0; ++i)
      str[i] = char_table[toBase(tmp, base, tmp)];
    for (int j = i - 1; j >= 0; --j)
      printf("%c", str[j]);
    printf("\n");
    free(tmp);
    free(str);
  }
}

uint toBase(const Integer *left, uint right, Integer *dst) {
  uint remainder = 0;
  ulong div = 0;
  uint *tmp = (uint *)(&div);

  Integer *new_left = (Integer *)malloc(g_size_integer);
  memcpy(new_left, left, g_size_integer);

  for (int i = new_left->length - 1; i >= 0; --i) {
    tmp[0] = new_left->data[i];
    tmp[1] = remainder;
    dst->data[i] = (uint)(div / (ulong)right);
    remainder = (uint)(div - (ulong)(dst->data[i]) * (ulong)right);
  }

  dst->length = new_left->length;
  while (dst->data[dst->length - 1] == 0 && dst->length > 1)
    dst->length -= 1;

  free(new_left);
  return remainder;
}
inline Integer *toInteger(int n) {
  Integer *dst = (Integer *)malloc(g_size_integer);
  memset(dst, 0, g_size_integer);
  if (n < 0) {
    n = -n;
    dst->sign = 1;
  }
  dst->length = 1;
  dst->data[0] = (uint)(n);
  return dst;
}
inline Integer *initInteger() {
  Integer *dst = (Integer *)malloc(g_size_integer);
  memset(dst, 0, g_size_integer);
  dst->length = 1;
  return dst;
}
inline Integer *l2Integer(long n) {
  Integer *dst = (Integer *)malloc(g_size_integer);
  memset(dst, 0, g_size_integer);
  if (n < 0) {
    n = -n;
    dst->sign = 1;
  }
  dst->length = 1;
  dst->data[0] = (uint)(n);
  if (n > 0xffffffff) {
    dst->length = 2;
    dst->data[1] = (uint)(n >> 32);
  }
  return dst;
}
