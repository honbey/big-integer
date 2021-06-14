#ifdef __cplusplus
extern "C"
#endif

#include "integer.h"

#include <time.h>


int main(int argc, char const *argv[]) {
  // srand((unsigned)time(0));

  Integer* src = generateInteger(1024);
  Integer* exp = generateInteger(256);
  Integer* mod = generateInteger(512);
  printInteger(src, 10);
  printInteger(exp, 10);
  printInteger(mod, 10);
  Integer* t = plusMinus(src, exp, 0);
//  Integer* r = modExponent(src, exp, mod);
//  Integer* r = modInversion(t, mod);
  Integer* r = gcdEuclid(src, mod);
  printInteger(r, 10);
  free(src);
  free(exp);
  free(mod);
  free(t);
  free(r);

  return 0;
}
