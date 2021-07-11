#ifdef __cplusplus
extern "C"
#endif

#include "integer.h"
#include "rsa.h"

#include <time.h>

void rsaTest(void);

int main(int argc, char const *argv[]) {
//  srand((unsigned)time(0));

  Integer** rsa_params = initRSAParams(1024, 3);

  char message[] = "Talk is cheap, show me the code.";
  int length = strlen(message);
  uchar* text = (uchar* )malloc(sizeof(uchar) * length);
  for (int i = 0; i < length; ++i) text[i] = (uchar)message[i];

  uchar* c = RSA(text, &length, rsa_params[1], rsa_params[0]);
  uchar* m = RSA(c, &length, rsa_params[2], rsa_params[0]);

  for (int i = 0; i < length; ++i) printf("%c", m[i]);
  printf("\n");

  free(text);
  free(c);
  free(m);

  for (int i = 0; i < 3; ++i) free(rsa_params[i]);
  free(rsa_params);

  return 0;
}

void rsaTest(void) {
  Integer** rsa_params = initRSAParams(1024, 3);
/*
* rsa_params[0]: n             (Public)
* rsa_params[1]: e Public  Key (Public)
* rsa_params[2]: d Private Key
*/

  Integer* message = toInteger(20210615);
  printf("Message   : ");
  printInteger(message, 10);
  Integer* ciphertext = modExponent(message, rsa_params[1], rsa_params[0]);
  printf("CipherText: ");
  printInteger(ciphertext, 10);
  Integer* plaintext = modExponent(ciphertext, rsa_params[2], rsa_params[0]);
  printf("PlainText : ");
  printInteger(plaintext, 10);

  for (int i = 0; i < 3; ++i) free(rsa_params[i]);
  free(rsa_params);

  free(message);
  free(ciphertext);
  free(plaintext);
}