#ifndef LIST_H
#define LIST_H
#include <cstddef>
#include <stdexcept>
#include <algorithm>

namespace my_cont {
    template<typename T>
    class Container {
    public:
        Container() = default;
        Container(const Container<T>& other) = default;

        //virtual Container<T>& operator=(const Container<T>& other) = 0;

        virtual ~Container() = default;

        [[nodiscard]] virtual bool empty() const = 0;
        [[nodiscard]] virtual std::size_t size() const = 0;
        [[nodiscard]] virtual std::size_t max_size() const = 0;
        //virtual bool operator==(const Container<T>& other) const = 0;
        //virtual bool operator!=(const Container<T>& other) const = 0;
    };

    template <class T, std::size_t N>
    class Array final : public Container<T>
    {
        template<class Type, std::size_t>
        friend class Array;
    protected:
        template <class IterType>
        class ArrayIterator
        {
        protected:
            friend class Array;
            using NoConstIterType = std::remove_const_t<IterType>;
            NoConstIterType *ptr = nullptr;
            explicit ArrayIterator(NoConstIterType *ptr) : ptr(ptr) {}
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
            explicit ArrayReverseIterator(NoConstIterType *ptr) : ptr(ptr) {}
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
        using NoConstT = std::remove_const_t<T>;
        NoConstT *arr = nullptr;

    public:
        using Iterator = ArrayIterator<NoConstT>;
        using ReverseIterator = ArrayReverseIterator<NoConstT>;
        using ConstIterator = ArrayIterator<const NoConstT>;
        using ConstReverseIterator = ArrayReverseIterator<const NoConstT>;

        Array() {
            this->arr = new NoConstT[N];
            this->cap = N;
            for (std::size_t i = 0; i < N; ++i) {
                this->arr[i] = 0;
            }
        }

        Array(std::initializer_list<NoConstT> init) {
            if (init.size() != N) {
                throw std::invalid_argument("Incorrect initializer list size.");
            }
            this->arr = new NoConstT[N];
            std::copy(init.begin(), init.end(), this->arr);
            this->cap = N;
        }

        Array(const Array &other) {
            cap = other.cap;
            arr = new NoConstT[cap];
            std::copy(other.arr, other.arr + cap, this->arr);
        }

        Array(Array &&other) noexcept {
            this->cap = other.cap;
            this->arr = other.arr;
            other.arr = nullptr;
            other.cap = 0;
        }

        Array& operator=(const Array& other) noexcept {
            if (&other != this) {
                delete[] arr;
                this->cap = other.cap;
                this->arr = new NoConstT[this->cap];
                for (size_t i = 0; i < cap; ++i)
                    arr[i] = other.arr[i];
            }
            return *this;
        }

        Array &operator=(Array &&other) noexcept {
            if (this != &other) {
                delete[] this->arr;
                this->cap = other.cap;
                this->arr = other.arr;
                other.arr = nullptr;
                other.cap = 0;
            }
            return *this;
        }


        ~Array() override
        {
            delete[] this->arr;
        }

        [[nodiscard]] std::size_t size() const noexcept override
        {
            return this->cap;
        }

        [[nodiscard]] std::size_t max_size() const noexcept override
        {
            return this->cap;
        }

        T& at(std::size_t ind)
        {
            if (ind < this->cap)
            {
                return this->arr[ind];
            }
            throw std::out_of_range("ind");
        }

        T& operator[](std::size_t ind)
        {
            return this->arr[ind];
        }

        T &front() {
            return this->arr[0];
        }

        T &back() {
            return this->arr[this->cap - 1];
        }

        T *data() {
            return this->arr;
        }

        Iterator begin()
        {
            return Iterator(this->arr);
        }
        Iterator end()
        {
            return Iterator(this->arr + this->cap);
        }

        ConstIterator cbegin() const
        {
            return ConstIterator(this->arr);
        }
        ConstIterator cend() const
        {
            return ConstIterator(this->arr + this->cap);
        }

        ReverseIterator rbegin()
        {
            return ReverseIterator(this->arr + this->cap - 1);
        }
        ReverseIterator rend()
        {
            return ReverseIterator(this->arr - 1);
        }

        ConstReverseIterator crbegin()
        {
            return ConstReverseIterator(this->arr + this->cap - 1);
        }
        ConstReverseIterator crend()
        {
            return ConstReverseIterator(this->arr - 1);
        }

        void fill(const T& value) {
            for (std::size_t i = 0; i < this->cap; ++i) arr[i] = value;
        }

        void swap(Array &other) noexcept {
            for (std::size_t i = 0; i < this->cap; i++) {
                NoConstT tmp = this->arr[i];
                this->arr[i] = other.arr[i];
                other.arr[i] = tmp;
            }
        }

        bool operator==(const Array& other) const
        {
            if (this->cap != other.cap) {
                return false;
            }
            for (std::size_t i = 0; i < this->cap; i++) {
                if (this->arr[i] != other.arr[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Array& other) const
        {
            return !(*this == other);
        }

        bool operator<(const Array& other)
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

        bool operator<=(const Array& other)
        {
            return (*this < other || *this == other);
        }

        bool operator>=(const Array& other)
        {
            return !(*this < other);
        }

        bool operator>(const Array &other) const {
            return !(*this <= other);
        }

        template<std::size_t N2>
            std::strong_ordering operator<=>(const Array<T, N2> &other) const {
            std::size_t n = std::min(N, N2);
            for (std::size_t i = 0; i < n; i++) {
                if (auto cmp = this->arr[i] <=> other.arr[i]; cmp != 0) {
                    return cmp;
                }
            }

            if (auto cmp = this->cap <=> other.cap; cmp != 0) {
                return cmp;
            }

            return std::strong_ordering::equal;
        }

        [[nodiscard]] bool empty() const noexcept override
        {
            return this->cap == 0;
        }
    };
}
#endif