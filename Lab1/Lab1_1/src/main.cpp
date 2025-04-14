#include <iostream>
#include "my_lib.h"

int main() {
    Array<int, 10> arr;
    arr[0] = 1;
    arr[1] = 2;
    arr[2] = 3;
    for (const auto &x : arr) {
        std::cout << x << std::endl;
    }
}
