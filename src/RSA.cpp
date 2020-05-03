/**
 * KRY - RSA
 * Tomas Willaschek
 * xwilla00
 */

#include <string>
#include <ctime>

#include "RSA.h"

using namespace std;


RSA::RSA() {}

RSA::~RSA() = default;

void RSA::generate(int bits) {
    gmp_randstate_t state;
    std::time_t t = std::time(0);
    gmp_randinit_default(state);
    gmp_randseed_ui(state, t);
    mpz_t p, q, n, e, tmp, tmp2, phi, d;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init_set_ui(e, 3);
    mpz_init(tmp);
    mpz_init(tmp2);
    mpz_init(phi);

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
    mpz_init(d);

    inv_mod(&d, &e, &phi);
    gmp_printf("%#Zx %#Zx %#Zx %#Zx %#Zx\n", p, q, n, e, d);

    mpz_clear(p);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(e);
    mpz_clear(phi);
    mpz_clear(d);
    mpz_clear(tmp);
    mpz_clear(tmp2);
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
            mpz_clear(e);
            mpz_clear(a);
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
    mpz_t g, h, y;
    mpz_t w, z, v, r;
    mpz_init_set_ui(w, 1);
    mpz_init_set_ui(z, 0);
    mpz_init_set_ui(v, 0);
    mpz_init_set_ui(r, 1);
    mpz_init_set(g, *phi);
    mpz_init_set(h, *e);
    mpz_init(y);

    while (mpz_cmp_ui(h, 0) > 0){
        mpz_div(y, g, h);
        update(&g, &h, &y);
        update(&w, &z, &y);
        update(&v, &r, &y);
    }
    mpz_mod(*dst, v, *phi);

    mpz_clear(g);
    mpz_clear(h);
    mpz_clear(y);
    mpz_clear(w);
    mpz_clear(z);
    mpz_clear(v);
    mpz_clear(r);
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

void RSA::update(mpz_t *a, mpz_t *b, mpz_t *y){
    mpz_t tmp;
    mpz_init_set(tmp, *b);

    mpz_mul(*b, *y, tmp);
    mpz_sub(*b, *a, *b);
    mpz_swap(tmp, *a);

    mpz_clear(tmp);
}