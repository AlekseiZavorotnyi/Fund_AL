#include <iostream>
#include "big_int.h"

int main() {
    const BigInt bigint("1111111111");
    const BigInt bigint1("-999999999");
    const BigInt bigint2 = bigint + bigint1;
    std::cout << bigint2 << std::endl;
}
