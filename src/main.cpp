#include <iostream>
#include <cstring>

#include "RSA.h"

using namespace std;

void help(){

}

int main(int argc, char **argv) {
    int bits;

    if(argc > 2){
        if(strcmp(argv[1], "-g") == 0){
            bits = atoi(argv[2]);
        }
        else if(strcmp(argv[1], "-e") == 0){

        }
        else if(strcmp(argv[1], "-d") == 0){

        }
        else if(strcmp(argv[1], "-b") == 0){

        }
        else{
            cerr << "Unknown parameters, use -h to show help" << endl;
            exit(1);
        }
    } else{
        cerr << "Too few args, use -h to show help" << endl;
        exit(1);
    }

    auto rsa = RSA();
    rsa.generate(bits);


    return 0;
}
