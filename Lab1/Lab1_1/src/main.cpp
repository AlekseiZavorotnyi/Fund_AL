#include <iostream>
#include "my_lib.h"

int main() {
    my_cont::Array<int, 10> arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    my_cont::Array<int, 10> arr1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 9};
    for (const auto &&x : arr) {
        std::cout << x << std::endl;
    }
    std::cout << (arr < arr1) << std::endl;
}
