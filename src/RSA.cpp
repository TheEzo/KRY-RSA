//
// Created by tezzo on 5/2/20.
//

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <random>
#include <string>

#include "RSA.h"

using namespace std;


RSA::RSA() {
    mpz_init(n);
    mpz_set_ui(n, 0);

}

RSA::~RSA() = default;

void RSA::generate(int bits) {
    gmp_randstate_t state;
    gmp_randinit_default(state);
    mpz_t p, q, n, e, tmp, tmp2, phi, d;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init_set_ui(e, 3);
    mpz_init(tmp);
    mpz_init(tmp2);
    mpz_init(phi);
    mpz_init(d);

    get_prime(state, &p, bits/2);
    get_prime(state, &q, bits/2);

    mpz_sub_ui(tmp, p, 1);
    mpz_sub_ui(tmp2, q, 1);
    mpz_mul(phi, tmp, tmp2);
    mpz_mul(n, p, q);

    while(true){
        gcd(&tmp, &e, &phi);
        if(mpz_cmp_ui(tmp, 1) > 0)
            mpz_add_ui(e, e, 1);
        else
            break;
    }
    inv_mod(&d, &e, &phi);
    gmp_printf("%#Zx %#Zx %#Zx %#Zx %#Zx\n", p, q, n, e, d);

    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(tmp);
    mpz_clear(tmp2);
    mpz_clear(phi);
    mpz_clear(d);

//    gmp_printf("%Zd\n", p);
//    gmp_printf("%#Zx\n", p);
//    gmp_printf("%Zd\n", q);
//    gmp_printf("%#Zx\n", q);
//
//
//    gmp_printf("%Zd\n", n);
//    gmp_printf("%#Zx\n", n);


}


void RSA::get_prime(gmp_randstate_t s, mpz_t *dst, int bits) {
    bool prime = false;
    mpz_t tmp;
    mpz_t tmp2;
    mpz_t tmp3;
    mpz_init(tmp);
    mpz_init(tmp2);
    mpz_init(tmp3);
    while(!prime){
        mpz_urandomb(*dst, s, bits);
        while(mpz_cmp_ui(*dst, 0) == 0)
            mpz_urandomb(*dst, s, bits);
        mpz_set(tmp3, *dst);
        prime = true;
        for(int i = 0; i < 100; i++){
            mpz_urandomb(tmp, s, bits-1);
            gcd(&tmp2, &tmp3, &tmp);
            if(mpz_cmp_ui(tmp2, 1) > 0){
                prime = false;
                break;
            }
        }
        if(prime){
            prime = lehmann(dst, 10, s, bits);
        }
    }
}

bool RSA::lehmann(mpz_t *n, int k, gmp_randstate_t s, int bits) {
    mpz_t a;
    mpz_init(a);
    mpz_urandomb(a, s, bits-1);
    mpz_t e;
    mpz_init(e);
    mpz_sub_ui(e, *n, 1);
    mpz_div_ui(e, e, 2);

    mpz_t res, tmp, tmp2;
    mpz_init(res);
    mpz_init(tmp);
    mpz_init(tmp2);
    while(k > 0){
        mpz_powm(res, a, e, *n);
        mpz_mod(tmp, res, *n);
        mpz_sub_ui(tmp2, *n, 1);
        if(mpz_cmp_ui(tmp, 1) == 0 || mpz_cmp(tmp, tmp2) == 0){
            mpz_urandomb(a, s, bits-1);
            k--;
        }
        else{
            mpz_clear(res);
            mpz_clear(tmp);
            mpz_clear(tmp2);
            return false;
        }
    }
    return true;
}

void RSA::gcd(mpz_t *dst, mpz_t *a, mpz_t *b) {
    if(mpz_cmp_ui(*b, 0) == 0){
        mpz_swap(*dst, *a);
        return;
    }
    mpz_t tmp;
    mpz_init(tmp);
    mpz_mod(tmp, *a, *b);
    return gcd(dst, b, &tmp);
}

void RSA::inv_mod(mpz_t *dst, mpz_t *e, mpz_t *phi) {
    mpz_t i;
    mpz_t a;
    mpz_t tmp;
    mpz_init(a);
    mpz_init(tmp);
    mpz_mod(a, *e, *phi);

    mpz_init_set_ui(i, 1);
    while(mpz_cmp(i, *phi) < 0){
        mpz_mul(tmp, a, i);
        mpz_mod(tmp, tmp, *phi);
        if(mpz_cmp_ui(tmp, 1) == 0){
            mpz_set(*dst, i);
            return;
        }
        mpz_add_ui(i, i, 1);
    }
}

void RSA::encrypt(char **argv) {
    mpz_t e, n, m, enc;
    string in = argv[2];
    in = in.substr(2,in.size());
    mpz_init_set_str(e, in.c_str(), 16);
    in = argv[3];
    in = in.substr(2,in.size());
    mpz_init_set_str(n, in.c_str(), 16);
    in = argv[4];
    in = in.substr(2,in.size());
    mpz_init_set_str(m, in.c_str(), 16);

    mpz_init(enc);
    mpz_powm(enc, m, e, n);
    gmp_printf("%#Zx\n", enc);

    mpz_clear(e);
    mpz_clear(n);
    mpz_clear(m);
    mpz_clear(enc);
}

void RSA::decrypt(char **argv) {
    mpz_t d, n, c, dec;
    string in = argv[2];
    in = in.substr(2,in.size());
    mpz_init_set_str(d, in.c_str(), 16);
    in = argv[3];
    in = in.substr(2,in.size());
    mpz_init_set_str(n, in.c_str(), 16);
    in = argv[4];
    in = in.substr(2,in.size());
    mpz_init_set_str(c, in.c_str(), 16);

    mpz_init(dec);
    mpz_powm(dec, c, d, n);
    gmp_printf("%#Zx\n", dec);

    mpz_clear(d);
    mpz_clear(n);
    mpz_clear(c);
    mpz_clear(dec);
}