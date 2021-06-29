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
