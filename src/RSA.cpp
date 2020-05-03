//
// Created by tezzo on 5/2/20.
//

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <random>

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
    mpz_t p, q, n;
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    get_prime(state, &p, bits/2);
    get_prime(state, &q, bits/2);

//    gmp_printf("%#Zx %#Zx %#Zx %#Zx %#Zx", p, q, n, e, d);

//    mpz_set_ui(p, 256);
//    mpz_urandomb(p, state, 12);
    gmp_printf("%Zd\n", p);
    gmp_printf("%#Zx\n", p);
    gmp_printf("%Zd\n", q);
    gmp_printf("%#Zx\n", q);

    mpz_mul(n, p, q);
    gmp_printf("%Zd\n", n);
    gmp_printf("%#Zx\n", n);


//    unsigned int p = 13;//get_prime();
//    unsigned int q = 11;//get_prime();
//    double n = p*q;
//    unsigned int e = 7;
//
////    cout << "bits: " << (int)log2(n) +1 << endl;
//    int msg = 111;
////    int c = pow(msg, e);
////    int enc = fmod(c, n);
////
////    int d = fmod(1/e, (p-1)*(q-1));
////    int m = pow(pow(c,d), n);
//    double track;
//    double phi = (p-1)*(q-1);
//    while(e<phi) {
//        track = gcd(e,phi);
//        if(track==1)
//            break;
//        else
//            e++;
//    }
//    double d1=1/e;
//    double d=fmod(d1,phi);
//    double c = pow(msg,e); //encrypt the message
//    double m = pow(c,d);
//    c=fmod(c,n);
//    m=fmod(m,n);
//
//    cout << "MSG: " << msg << endl;
//    cout << "Encrypted: " << c << endl;
//    cout << "Decrypted: " << m <<  endl;


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

//int RSA::jacobi(int a, int n){
//    if(gcd(a, n) != 1)
//        return 0;
//    int negative = 1;
//    while(a > 1){
//        if(n > a && a % 2 == 1 && n % 2 == 1){
//            int tmp = a;
//            a = n;
//            n = tmp;
//            if((a % 4) == 3 && (n % 4) == 3)
//                negative *= -1;
//            a = a % n;
//        }
//        else if(n % 2 == 0){
//            n = n/2;
//            negative *= (n % 8) == 3 || n % 8 == 5 ? -1 : 1;
//        } else
//            return 0;
//    }
//    return negative;
//}