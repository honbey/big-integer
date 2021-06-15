#ifdef __cplusplus
extern "C"
#endif

#include "integer.h"

#include <time.h>

void rsaTest(Integer* p, Integer* q);

int main(int argc, char const *argv[]) {
//  srand((unsigned)time(0));

  char src_s[] = "179077962644763988778454871311384943879524311577092920038471272892668598829385732513147226149528425459497198997559301306399601130167764394144278060046068700696669742838771413656706529807583111711198608251679508540119288790046086555285873887672166371652326121406121294656498215821821914877792690850603578106583";
  char mod_s[] = "9483628978368866175585758312414987507013780675811167995084250495497528067430915741368411259986075625615909863743529766472040767409604321316964381041761154907328559714450387479529803381760506734417933336481311868566375855034288687928329856647248971122010990587044820013034655257307084442479";
  Integer* src = str2Integer(src_s);
//  Integer* exp = generateInteger(256);
  Integer* mod = str2Integer(mod_s);
  printInteger(src, 10);
//  printInteger(exp, 10);
  printInteger(mod, 10);
//  Integer* t = plusMinus(src, exp, 0);
//  Integer* r = modExponent(src, exp, mod);
//  Integer* r = modInversion(t, mod);
//  Integer* r = generateInteger(256);
//  printInteger(r, 10);
  rsaTest(src, mod);
  free(src);
//  free(exp);
  free(mod);
//  free(t);
//  free(r);

  return 0;
}

void rsaTest(Integer* p, Integer* q) {
  Integer* one = toInteger(1);
  printf("\nrsa\n");
  printf("p  : ");
  printInteger(p, 10);
  printf("q  : ");
  printInteger(q, 10);

  Integer* p_1 = plusMinus(p, one, 1);
  printf("p-1: ");
  printInteger(p_1, 10);
  Integer* q_1 = plusMinus(q, one, 1);
  printf("q-1: ");
  printInteger(q_1, 10);
  Integer* n = mulDiv(p, q, 0, NULL);
  printf("n  : ");
  printInteger(n, 10);
  Integer* n_ = mulDiv(p_1, q_1, 0, NULL);
  printf("n_ : ");
  printInteger(n_, 10);
  Integer* e = toInteger(65537);
  printf("e  : ");
  printInteger(e, 10);
  Integer* d = modInversion(e, n_);
  printf("d  : ");
  printInteger(d, 10);
  Integer* m = toInteger(20210615);
  printf("m  : ");
  printInteger(m, 10);
  Integer* c = modExponent(m, e, n);
  printf("c  : ");
  printInteger(c, 10);
  Integer* d_ = modExponent(c, d, n);
  printf("d_ : ");
  printInteger(d_, 10);

  free(one);
  free(p_1);
  free(q_1);
  free(n);
  free(n_);
  free(e);
  free(d);
  free(m);
  free(c);
  free(d_);
}