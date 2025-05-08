#include <iostream>
#include "big_int.h"

int main() {
    BigInt bigint("1219496545132020383724120990446767044759");
    BigInt bigint1("98765432829729388731");
    std::cout << bigint/bigint1 << std::endl;
}
