#pragma once
#include <iostream>
#include <exception>
#include <cstddef>  // для std::size_t
#include <vector>
#include <stdexcept>
#include <algorithm>

/*namespace my_ex {
    class Ex final : std::exception {

    };
    void print() {
        std::cout << "WW" << std::endl;
    }
}
*/


template<typename T>
class Container {
public:
    Container() = default;
    Container(const Container<T>& other) = default;

    virtual Container<T>& operator=(const Container<T>& other) = 0;

    virtual ~Container() = default;

    virtual T* begin() = 0;
    virtual const T* cbegin() const = 0;
    virtual T* end() = 0;
    virtual const T* cend() const = 0;
    virtual bool empty() = 0;
    virtual std::size_t size() = 0;
    virtual std::size_t max_size() = 0;
    virtual bool operator==(const Container<T>& other) const = 0;
    virtual bool operator!=(const Container<T>& other) const = 0;
};


template <class T, std::size_t N>
class Array : public Container<T>
{
protected:
    template <class IterType>
    class ArrayIterator
    {
    protected:
        friend class Array;
        using NoConstIterType = std::remove_const_t<IterType>;
        NoConstIterType *ptr = nullptr;
        ArrayIterator(NoConstIterType *ptr) : ptr(ptr) {}
        ArrayIterator(const ArrayIterator &other) = default;

    public:
        ArrayIterator(ArrayIterator &&other) = default;
        ArrayIterator operator++()
        {
            this->ptr++;
            return *this;
        }

        ArrayIterator operator++(int)
        {
            ArrayIterator tmp = *this;
            ++this->ptr;
            return tmp;
        }

        ArrayIterator operator--()
        {
            this->ptr--;
            return *this;
        }

        ArrayIterator operator--(int)
        {
            ArrayIterator tmp = *this;
            --this->ptr;
            return tmp;
        }

        bool operator==(const ArrayIterator &other) const
        {
            return this->ptr == other.ptr;
        }
        bool operator!=(const ArrayIterator &other) const
        {
            return !(*this == other);
        };
        IterType operator*()
        {
            return *this->ptr;
        }
    };

    template <class IterType>
    class ArrayReverseIterator
    {
    protected:
        friend class Array;
        using NoConstIterType = std::remove_const_t<IterType>;
        NoConstIterType *ptr = nullptr;
        ArrayReverseIterator(NoConstIterType *ptr) : ptr(ptr) {}
        ArrayReverseIterator(const ArrayReverseIterator &other) = default;

    public:
        ArrayReverseIterator(ArrayReverseIterator &&other) = default;
        ArrayReverseIterator operator++() override
        {
            this->ptr--;
            return *this;
        }
        ArrayReverseIterator operator++(int) override
        {
            ArrayReverseIterator tmp = *this;
            --this->ptr;
            return tmp;
        }

        ArrayReverseIterator operator--() override
        {
            this->ptr++;
            return *this;
        }

        ArrayReverseIterator operator--(int) override
        {
            ArrayReverseIterator tmp = *this;
            ++this->ptr;
            return tmp;
        }

        bool operator==(const ArrayReverseIterator &other) const override
        {
            return this->ptr == other.ptr;
        }
        bool operator!=(const ArrayReverseIterator &other) const override
        {
            return !(*this == other);
        };
        IterType operator*() override
        {
            return *this->ptr;
        }
    };

    std::size_t len = 0;
    std::size_t cap = 0;
    T *arr = nullptr;

public:
    Array() : cap(N), arr(new T[this->cap]) {}

    Array(const std::initializer_list<T>& list) {
        if(list.size() > N) throw std::out_of_range("Initializer list is too large");
        std::copy_n(list.begin(), list.size(), arr);
        this->len = list.size();
    }

    Array<T, N>& operator=(const Container<T>& other) override {
        if (&other != this) {
            delete[] arr;
            len = other.size;
            arr = new T[size];
            for (size_t i = 0; i < len; ++i)
                arr[i] = other.arr[i];
        }
        return *this;
    }

    ~Array() override
    {
        delete[] this->arr;
    }

    std::size_t size() override
    {
        return this->len;
    }

    std::size_t max_size() override
    {
        return this->cap;
    }

    T at(std::size_t ind)
    {
        if (ind < this->len)
        {
            return this->arr[ind];
        }
        else
        {
            throw std::out_of_range("ind");
        }
    }

    T operator[](std::size_t ind)
    {
        return this->arr[ind];
    }

    ArrayIterator<T> begin() override
    {
        return ArrayIterator<T>(this->arr);
    }
    ArrayIterator<T> end() override
    {
        return ArrayIterator<T>(this->arr + this->len);
    }

    ArrayIterator<const T> cbegin() const override
    {
        return ArrayIterator<const T>(this->arr);
    }
    ArrayIterator<const T> cend() const override
    {
        return ArrayIterator<const T>(this->arr + this->len);
    }

    ArrayReverseIterator<T> rbegin()
    {
        return ArrayReverseIterator<T>(this->arr + this->len - 1);
    }
    ArrayReverseIterator<T> rend()
    {
        return ArrayReverseIterator<T>(this->arr - 1);
    }

    ArrayReverseIterator<const T> crbegin()
    {
        return ArrayReverseIterator<const T>(this->arr + this->len - 1);
    }
    ArrayReverseIterator<const T> crend()
    {
        return ArrayReverseIterator<const T>(this->arr - 1);
    }

    void fill(const T& value) {
        for (std::size_t i = 0; i < this->len; ++i) arr[i] = value;
    }

    bool operator==(const Container<T> & other) const override
    {
        if (this->len != other.len) {
            return false;
        }
        for (int i = 0; i < this->len; i++) {
            if (this[i] != other[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Container<T> & other) const override
    {
        if (this->len != other.len) {
            return true;
        }
        for (int i = 0; i < this->len; i++) {
            if (this[i] != other[i]) {
                return true;
            }
        }
        return false;
    }

    bool operator<(Array<T, N>& other)
    {
        if (this->len != other.len) {
            return this->len < other.len;
        }
        for (int i = 0; i < this->len; i++) {
            if (this[i] != other[i]) {
                return this[i] < other[i];
            }
        }
        return false;
    }

    bool operator>(Array<T, N>& other)
    {
        if (this->len != other.len) {
            return this->len > other.len;
        }
        for (int i = 0; i < this->len; i++) {
            if (this[i] != other[i]) {
                return this[i] > other[i];
            }
        }
        return false;
    }

    bool operator<=(Array<T, N>& other)
    {
        if (this->len != other.len) {
            return this->len <= other.len;
        }
        for (int i = 0; i < this->len; i++) {
            if (this[i] != other[i]) {
                return this[i] <= other[i];
            }
        }
        return true;
    }

    bool operator>=(Array<T, N>& other)
    {
        if (this->len != other.len) {
            return this->len >= other.len;
        }
        for (int i = 0; i < this->len; i++) {
            if (this[i] != other[i]) {
                return this[i] >= other[i];
            }
        }
        return true;
    }

    /*bool operator<=>(Array<T, N>& other)
    {
        if (this->len != other.len) {
            return this->len <= other.len;
        }
        for (int i = 0; i < this->len; i++) {
            if (this[i] != other[i]) {
                return this[i] <= other[i];
            }
        }
        return true;
    }*/

    bool empty() override
    {
        return this->size == 0;
    }
};