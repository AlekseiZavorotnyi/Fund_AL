#include <iostream>
#include "big_int.h"

int main() {
    const BigInt bigint("948567934769353853598376");
    const BigInt bigint1("82376982732409329487287244");
    const BigInt bigint2 = bigint * bigint1;
    std::cout << bigint2 << std::endl;
}
