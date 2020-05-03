#include <iostream>
#include <cstring>

#include "RSA.h"

using namespace std;

int main(int argc, char **argv) {
    int bits;

    auto rsa = RSA();
    if(argc > 2){
        if(strcmp(argv[1], "-g") == 0){
            bits = atoi(argv[2]);
            rsa.generate(bits);
        }
        else if(strcmp(argv[1], "-e") == 0){
            if(argc == 5)
                rsa.encrypt(argv);
            else{
                cerr << "Incorrect arg count" << endl;
            }
        }
        else if(strcmp(argv[1], "-d") == 0){
            if(argc == 5)
                rsa.decrypt(argv);
            else
                cerr << "Incorrect arg count" << endl;
        }
        else if(strcmp(argv[1], "-b") == 0){
            exit(0);
        }
        else{
            cerr << "Unknown parameters, use -h to show help" << endl;
            exit(1);
        }
    } else{
        cerr << "Too few args, use -h to show help" << endl;
        exit(1);
    }
    return 0;
}
