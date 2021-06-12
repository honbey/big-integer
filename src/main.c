#ifdef __cplusplus
extern "C"
#endif

#include "integer.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


int main(int argc, char const *argv[]) {
  // srand((unsigned)time(0));

  Integer *n = generateInteger(1024);
  Integer *m = generateInteger(512);
  printInteger(n, 10);
  printInteger(m, 10);
  Integer *r = mulDiv(n, m, 1);
  printInteger(r, 10);
  free(n);
  free(m);
  free(r);

  return 0;
}
