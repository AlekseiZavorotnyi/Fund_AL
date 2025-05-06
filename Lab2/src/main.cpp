#include <iostream>
#include "big_int.h"

int main() {
    BigInt bigint("123456789");
    BigInt bigint1("987654321");
    bigint += bigint1;
    std::cout << bigint << std::endl;
}
