#include <iostream>
#include "big_int.h"

int main() {
    BigInt bigint("3");
    BigInt bigint1("18");
    std::cout << bigint.mod_exp(bigint1, BigInt(1024)) << std::endl;
}
