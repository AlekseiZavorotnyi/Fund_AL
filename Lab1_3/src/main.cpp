#include <iostream>
#include "deque.h"

int main() {
    my_cont::List<int> a = {1, 2, 3};
    std::cout << a.empty() << std::endl;
}
