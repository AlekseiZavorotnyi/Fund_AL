#include <iostream>
#include "big_int.h"

int main() {
    BigInt bigint;
    std::cin >> bigint;
    BigInt bigint1("224432534754");
    BigInt bigint2 = bigint / bigint1;
    std::cout << bigint2 << std::endl;
}
