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

    //virtual Container<T>& operator=(const Container<T>& other) = 0;

    virtual ~Container() = default;

    virtual bool empty() = 0;
    virtual std::size_t size() = 0;
    virtual std::size_t max_size() = 0;
    //virtual bool operator==(const Container<T>& other) const = 0;
    //virtual bool operator!=(const Container<T>& other) const = 0;
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
        ArrayReverseIterator operator++()
        {
            this->ptr--;
            return *this;
        }
        ArrayReverseIterator operator++(int)
        {
            ArrayReverseIterator tmp = *this;
            --this->ptr;
            return tmp;
        }

        ArrayReverseIterator operator--()
        {
            this->ptr++;
            return *this;
        }

        ArrayReverseIterator operator--(int)
        {
            ArrayReverseIterator tmp = *this;
            ++this->ptr;
            return tmp;
        }

        bool operator==(const ArrayReverseIterator &other) const
        {
            return this->ptr == other.ptr;
        }
        bool operator!=(const ArrayReverseIterator &other) const
        {
            return !(*this == other);
        };
        IterType operator*()
        {
            return *this->ptr;
        }
    };

    std::size_t cap = 0;
    T *arr = nullptr;

public:
    // Array() : cap(N), arr(new T[this->cap]) {}
    //
    // Array(const std::initializer_list<T>& list) {
    //     if(list.size() > N) throw std::out_of_range("Initializer list is too large");
    //     arr = new T[this->cap];
    //     std::copy_n(list.begin(), list.size(), arr);
    //     this->cap = list.size();
    // }
    Array() : cap(N), arr(new T[cap]) {}
    explicit Array(const std::size_t len, const T& val) :
    cap(N), arr(new T[len]) {
        for (std::size_t i = 0; i < len && i < cap; i++) {
            arr[i] = val;
        }
    }
    Array(const Array &other) :
    cap(other.cap), arr(new T[cap]) {
        std::copy(other.arr, other.arr + cap, arr);
    };
    Array(Array &&other)  noexcept :
    cap(other.cap), arr(other.arr) {
        other.arr = nullptr;
        other.cap = 0;
    }
    Array(std::initializer_list<T> init) :
    cap(N), arr(new T[cap]) {
        std::size_t i = 0;
        for (const auto el : init) {
            if (i >= cap) break;
            arr[i++] = el;
        }
    }
    Array(std::initializer_list<const std::pair<size_t, T>> init) :
    cap(N), arr(new T[cap]) {
        for (const auto& [ind, val] : init) {
            if (ind < cap) {
                arr[ind] = val;
            }
        }
    }

    Array& operator=(const Array& other) {
        if (&other != this) {
            delete[] arr;
            this->cap = other.cap;
            this->arr = new T[this->cap];
            for (size_t i = 0; i < cap; ++i)
                arr[i] = other.arr[i];
        }
        return *this;
    }

    ~Array() final
    {
        delete[] this->arr;
    }

    std::size_t size() final
    {
        return this->cap;
    }

    std::size_t max_size() final
    {
        return this->cap;
    }

    T at(std::size_t ind)
    {
        if (ind < this->cap)
        {
            return this->arr[ind];
        }
        throw std::out_of_range("ind");
    }

    T operator[](std::size_t ind)
    {
        return this->arr[ind];
    }

    ArrayIterator<T> begin()
    {
        return ArrayIterator<T>(this->arr);
    }
    ArrayIterator<T> end()
    {
        return ArrayIterator<T>(this->arr + this->cap);
    }

    ArrayIterator<const T> cbegin() const
    {
        return ArrayIterator<const T>(this->arr);
    }
    ArrayIterator<const T> cend() const
    {
        return ArrayIterator<const T>(this->arr + this->cap);
    }

    ArrayReverseIterator<T> rbegin()
    {
        return ArrayReverseIterator<T>(this->arr + this->cap - 1);
    }
    ArrayReverseIterator<T> rend()
    {
        return ArrayReverseIterator<T>(this->arr - 1);
    }

    ArrayReverseIterator<const T> crbegin()
    {
        return ArrayReverseIterator<const T>(this->arr + this->cap - 1);
    }
    ArrayReverseIterator<const T> crend()
    {
        return ArrayReverseIterator<const T>(this->arr - 1);
    }

    void fill(const T& value) {
        for (std::size_t i = 0; i < this->cap; ++i) arr[i] = value;
    }

    void swap(Array &other) noexcept {
        std::swap(arr, other.arr);
        std::swap(cap, other.cap);
    }


    bool operator==(const Array& other) const
    {
        if (this->cap != other.cap) {
            return false;
        }
        for (int i = 0; i < this->cap; i++) {
            if (this[i] != other[i]) {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const Array& other) const
    {
        return !(*this == other);
    }

    bool operator<(Array& other)
    {
        const std::size_t less_len = (this->cap > other.cap) ? other.cap : this->cap;

        for (std::size_t i = 0; i < less_len; ++i) {
            if (this->arr[i] < other.arr[i]) {
                return true;
            }
            if (this->arr[i] > other.arr[i]) {
                return false;
            }
        }
        return this->cap < other.cap;
    }

    bool operator<=(Array& other)
    {
        return (*this < other || *this == other);
    }

    bool operator>=(Array& other)
    {
        return !(*this < other);
    }

    bool operator>(const Array &other) const {
        return !(*this <= other);
    }

    constexpr auto operator<=>(const Array &other) const {
        if (*this == other) {
            return std::weak_ordering::equivalent;
        }
        if (*this < other) {
            return std::weak_ordering::less;
        }
        return std::weak_ordering::greater;
    }
    /*bool operator<=>(Array<T, N>& other)
    {
        if (this->cap != other.cap) {
            return this->cap <= other.cap;
        }
        for (int i = 0; i < this->cap; i++) {
            if (this[i] != other[i]) {
                return this[i] <= other[i];
            }
        }
        return true;
    }*/

    bool empty() final
    {
        return this->cap == 0;
    }
};