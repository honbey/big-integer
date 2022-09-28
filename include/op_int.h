#ifndef OP_INT_H_
#define OP_INT_H_

#include "core.h"

void set2Int32(Integer *src, int n);

void addUint32(Integer *left, uint right);
void subUint32(Integer *left, uint right);
void mulUint32(Integer *left, uint right);
void divUint32(Integer *left, uint right);

#endif
