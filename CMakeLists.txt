# cmake_minimum_required(VERSION <specify CMake version here>)
project(KRY_RSA)

set(CMAKE_CXX_STANDARD 14)

find_library(gmp gmp PATHS "/usr/lib/x86_64-linux-gnu/openssl-1.0.2/engines")
add_executable(KRY_RSA src/main.cpp src/RSA.cpp src/RSA.h)
target_link_libraries(KRY_RSA gmp)
