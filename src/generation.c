#include "generation.h"
#include "operation.h"


Integer* generateInteger(int bits) {
  Integer* dst = (Integer* )malloc(g_size_integer);
  memset(dst, 0, g_size_integer);
  dst->length = 1;

  for (int i = 0; i < bits/BIN_EXP_BASE-1; ++i) {
    dst->length += 1;
    dst->data[i] = rand();
  }

  dst->data[bits/BIN_EXP_BASE-1] = rand() & 0xffffu + 0x10000u;
  dst->data[0] |= 0x1u;
  return dst;
}

void printInteger(const Integer* src, int base) {
  if (base < 0 || 16 < base) base = 16;
  if (src->length == 1) {
    if (src->sign) printf("%c", '-');
    printf("%u\n", src->data[0]);
  } else {
    if (src->sign) printf("-");

    Integer* tmp = (Integer* )malloc(g_size_integer);
    memcpy(tmp, src, g_size_integer);

    char char_table[] = "0123456789ABCDEF";
    size_t size = sizeof(char) * (size_t)(src->length * BIN_EXP_BASE * 0.33);
    char *str = (char *)malloc(size);
    memset(str, 0, sizeof(size));
    int i = 0;
    for (; tmp->length > 1 || tmp->data[0] != 0; ++i) 
      str[i] = char_table[modUint32(tmp, base, tmp)];
    for (int j = i-1; j >= 0; --j) printf("%c", str[j]);
    printf("\n");
    free(tmp);
    free(str);
  }
}

uint modUint32(const Integer* left, uint right, Integer* dst) {
  uint remainder = 0;
  ulong tmp = 0;
  uint *div = (uint *)(&tmp);

  Integer* new_left = (Integer* )malloc(g_size_integer);
  memcpy(new_left, left, g_size_integer);

  for (int i = new_left->length-1; i >= 0; --i) {
    div[0] = new_left->data[i];
    div[1] = remainder;
    dst->data[i] = (uint)(tmp / (ulong)right);
    remainder = (uint)(tmp - (ulong)(dst->data[i]) * (ulong)right);
  }

  dst->length = new_left->length;
  while (dst->data[dst->length-1] == 0 && dst->length > 1) dst->length -= 1;

  free(new_left);
  return remainder;
}
