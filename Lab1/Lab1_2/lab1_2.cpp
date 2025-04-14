#include <iostream>
#include <exception>
//#pragma once
#include <cstddef>  // для std::size_t
#include <vector>
#include <stdexcept>
#include <algorithm>

namespace my_ex {
    class Ex final : std::exception {

    };
    void print() {
        std::cout << "WW" << std::endl;
    }
}

template<typename T>
class Container {
public:
    Container() = default;
    Container(const Container &p) = default;
    virtual ~Container() = default;

};


int main()
{
    my_ex::print();
}