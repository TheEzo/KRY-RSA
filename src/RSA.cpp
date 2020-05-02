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
    random_device rd;
    mt19937_64 gen(rd());
    int num;
//    while(isp)
    int p;
    int q;
    int b; // random
    //4.1.1 Choice of p and q.
    // vector 100 prvku a hledam, jeslti je vsude gcd s "b" 1, pokud jo, je to prvocislo
    // jakobiho symbol "a", a nalezi to Zn*
    // appendix J
    //(a/b) == a(b-1)/2 (mod b).

    int x = 10;
    cout << "Prime: " << gcd(100, 90) << endl;

}

bool RSA::is_prime(int a, int n) {
    if(gcd(a, n) > 1)
        return false;
    if((a/n) == pow(a, (n-1)/2 % n))
        return true;
    return false;
}

int RSA::gcd(int a, int b) {
    if(b == 0)
        return a;
    return gcd(b, a % b);
}

