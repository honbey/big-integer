#include "rsa.h"
#include "generation.h"
#include "operation.h"
#include "op_int.h"

Integer** initRSAParams(int bits, int args) {
  Integer* *params = (Integer** )malloc(sizeof(Integer* ) * 3);
  Integer* p = generatePrimeInteger(bits >> 1);
  Integer* q = generatePrimeInteger(bits >> 1);
  /* n = pq */
  params[0] = mulDiv(p, q, 0, NULL);
  /* p = p-1, q = q-1 */
  subUint32(p, 1u);
  subUint32(q, 1u);
  /* $phi$(n) = (p-1)(q-1) */
  Integer* n_ = mulDiv(p, q, 0, NULL);
  /* public key,  e */
  /* you can choose 65537 as public key by `params[1] = toInteger(65537);` */
  params[1] = generatePrimeInteger(bits >> 2);
  /* private key, d */
  params[2] = modInversion(params[1], n_);

  if (args & VERBOSE) {
    int base = (args & DECIMAL) ? 10 : 16;
    printf("***  RSA parameters  ***\n");
    printf("[p-1] (p is big prime): ");
    printInteger(p, base);
    printf("[q-1] (q is big prime): ");
    printInteger(q, base);
    printf("[n] = pq              : ");
    printInteger(params[0], base);
    printf("[phi(n)] = (p-1)(q-1) : ");
    printInteger(n_, base);
    printf("[e] (e is public key) : ");
    printInteger(params[1], base);
    printf("[d] (d is private key): ");
    printInteger(params[2], base);
    printf("************************\n");
  }

  free(p);
  free(q);
  free(n_);

  return params;
}

uchar* RSA(const uchar* message,
           int* length,
           const Integer* key,
           const Integer* n) {
  int int_length = *length / 4 + ((*length % 4) ? 1 : 0);

  Integer* int_text = initInteger();
  int_text->length = int_length;
  memcpy(int_text->data, message, sizeof(uchar) * *length);

  Integer* sub_text = modExponent(int_text, key, n);

  uint s = sub_text->data[sub_text->length-1];
  int l = 1;
  if (s > 0x00ffffffu) l = 4;
  else if (s > 0x0000ffffu) l = 3;
  else if (s > 0x000000ffu) l = 2;

  *length = (sub_text->length-1) * 4 + l;

  uchar* sub_message = (uchar* )malloc(sizeof(uchar) * *length);
  memcpy(sub_message, sub_text->data, sizeof(uchar) * *length);

  free(int_text);
  free(sub_text);
  return sub_message;
}

uchar* char2Uchar(const char* message, int length) {
  uchar* text = (uchar* )malloc(sizeof(uchar) * length);
  for (int i = 0; i < length; ++i) text[i] = (uchar)message[i];
  return text;
}