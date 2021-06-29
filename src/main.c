#ifdef __cplusplus
extern "C"
#endif

#include "integer.h"
#include "rsa.h"

#include <time.h>

void rsaTest(void);

int main(int argc, char const *argv[]) {
//  srand((unsigned)time(0));

  rsaTest();

  return 0;
}

void rsaTest(void) {
  Integer** rsa_params = NULL;
/*
* rsa_params[0]: n             (Public)
* rsa_params[1]: e Public  Key (Public)
* rsa_params[2]: d Private Key
*/
  rsa_params = initRSAParams(1024, 3);

  Integer* message = toInteger(20210615);
  printf("Message   : ");
  printInteger(message, 10);
  Integer* cipherText = modExponent(message, rsa_params[1], rsa_params[0]);
  printf("CipherText: ");
  printInteger(cipherText, 10);
  Integer* plainText = modExponent(cipherText, rsa_params[2], rsa_params[0]);
  printf("PlainText : ");
  printInteger(plainText, 10);

  for (int i = 0; i < 3; ++i) free(rsa_params[i]);
  free(rsa_params);

  free(message);
  free(cipherText);
  free(plainText);
}