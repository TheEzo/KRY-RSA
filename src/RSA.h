//
// Created by tezzo on 5/2/20.
//

#ifndef KRY_RSA_RSA_H
#define KRY_RSA_RSA_H

#include <gmpxx.h>


class RSA {
public:
    RSA();
    ~RSA();

    void generate(int bits);
    bool is_prime(int a, int n);

private:
    int gcd(int a, int b);

    mpz_class p;
    mpz_class q;
    mpz_t n;
    mpz_class phi;

};


#endif //KRY_RSA_RSA_H
