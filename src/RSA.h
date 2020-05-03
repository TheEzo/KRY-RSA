/**
 * KRY - RSA
 * Tomas Willaschek
 * xwilla00
 */

#ifndef KRY_RSA_RSA_H
#define KRY_RSA_RSA_H

#include <gmpxx.h>
#include <random>

using namespace std;

class RSA {
public:
    RSA();
    ~RSA();

    void generate(int bits);
    void encrypt(char **argv);
    void decrypt(char **argv);

private:
    void update(mpz_t *a, mpz_t *b, mpz_t *y);
    void get_prime(gmp_randstate_t s, mpz_t *dst, int bits);
    void gcd(mpz_t *dst, mpz_t *a, mpz_t *b);
    bool lehmann(mpz_t *n, int k, gmp_randstate_t s, int bits);
    void inv_mod(mpz_t *dst, mpz_t *e, mpz_t *phi);

};


#endif //KRY_RSA_RSA_H
