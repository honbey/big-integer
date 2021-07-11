#ifdef __cplusplus
extern "C"
#endif

#include "integer.h"
#include "rsa.h"

#include <time.h>

void rsaTest(int flag);

int main(int argc, char const *argv[]) {
//  srand((unsigned)time(0));

  rsaTest(2);

  return 0;
}

void rsaTest(int flag) {
  Integer** rsa_params = initRSAParams(1024, VERBOSE | DECIMAL);
/*
* rsa_params[0]: n             (Public)
* rsa_params[1]: e Public  Key (Public)
* rsa_params[2]: d Private Key
*/

  if (flag == 1) {
    Integer* message = toInteger(20210615);
    printf("Message   : ");
    printInteger(message, 10);
    Integer* ciphertext = modExponent(message, rsa_params[1], rsa_params[0]);
    printf("CipherText: ");
    printInteger(ciphertext, 10);
    Integer* plaintext = modExponent(ciphertext, rsa_params[2], rsa_params[0]);
    printf("PlainText : ");
    printInteger(plaintext, 10);

    free(message);
    free(ciphertext);
    free(plaintext);
  } else if (flag == 2) {
    char message[] = "Talk is cheap, show me the code.";
    int length = strlen(message);
    uchar* text = char2Uchar(message, length);

    uchar* c = RSA(text, &length, rsa_params[1], rsa_params[0]);
    uchar* m = RSA(c, &length, rsa_params[2], rsa_params[0]);

    for (int i = 0; i < length; ++i) printf("%c", m[i]);
    printf("\n");

    free(text);
    free(c);
    free(m);
  }

  for (int i = 0; i < 3; ++i) free(rsa_params[i]);
  free(rsa_params);
}