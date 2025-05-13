#include <iostream>
#include "big_int.h"

int main() {
    BigInt bigint("3");
    BigInt bigint1("18");
    BigInt bigint2 = bigint.karatsuba_multiply(bigint1);
    std::cout << bigint2 << std::endl;
}
