#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <stdexcept>
#include <algorithm>
#include <initializer_list>
#include <compare>

namespace my_cont {
    template<typename T>
    class Container {
    public:
        Container() = default;
        Container(const Container<T>& other) = default;

        virtual ~Container() = default;

        [[nodiscard]] virtual bool empty() const = 0;
        [[nodiscard]] virtual size_t size() const = 0;
    };

    template<typename T>
    class Vector : public Container<T> {
    private:
        T* data_ = nullptr;
        size_t len = 0;
        size_t cap = 0;
        static constexpr size_t DEFAULT_CAPACITY = 16;

        void reallocate(size_t new_capacity) {
            T* new_data = new T[new_capacity];
            for (size_t i = 0; i < len; ++i) {
                new_data[i] = std::move(data_[i]);
            }
            delete[] data_;
            data_ = new_data;
            cap = new_capacity;
        }

    public:
        Vector() = default;

        Vector(std::initializer_list<T> init) {
            reserve(init.size());
            for (const auto& item : init) {
                push_back(item);
            }
        }

        Vector(const Vector& other) {
            reserve(other.len);
            for (size_t i = 0; i < other.len; ++i) {
                push_back(other.data_[i]);
            }
        }

        Vector(Vector&& other) noexcept
            : data_(other.data_), len(other.len), cap(other.cap) {
            other.data_ = nullptr;
            other.len = 0;
            other.cap = 0;
        }

        ~Vector() override {
            delete[] data_;
        }

        Vector& operator=(const Vector& other) {
            if (this != &other) {
                clear();
                reserve(other.len);
                for (size_t i = 0; i < other.len; ++i) {
                    push_back(other.data_[i]);
                }
            }
            return *this;
        }

        Vector& operator=(Vector&& other) noexcept {
            if (this != &other) {
                delete[] data_;
                data_ = other.data_;
                len = other.len;
                cap = other.cap;
                other.data_ = nullptr;
                other.len = 0;
                other.cap = 0;
            }
            return *this;
        }

        T& operator[](size_t pos) {
            return data_[pos];
        }

        T& at(size_t pos) {
            if (pos >= len) {
                throw std::out_of_range("Vector::at - index out of range");
            }
            return data_[pos];
        }

        T& front() {
            if (empty()) throw std::out_of_range("Vector::front - empty vector");
            return data_[0];
        }

        T& back() {
            if (empty()) throw std::out_of_range("Vector::back - empty vector");
            return data_[len - 1];
        }

        T* data() noexcept {
            return data_;
        }

        [[nodiscard]] bool empty() const override {
            return len == 0;
        }

        [[nodiscard]] size_t size() const override {
            return len;
        }

        [[nodiscard]] size_t capacity() const {
            return cap;
        }

        void reserve(size_t new_capacity) {
            if (new_capacity > cap) {
                reallocate(new_capacity);
            }
        }

        void shrink_to_fit() {
            if (len < cap) {
                reallocate(len);
            }
        }

        void clear() {
            if (len == 0) {
                return;
            }
            for (size_t i = 0; i < len; i++) {
                data_[i].~T();
            }
            len = 0;
        }

        void push_back(const T& value) {
            if (len >= cap) {
                reserve(cap == 0 ? DEFAULT_CAPACITY : cap * 2);
            }
            data_[len++] = value;
        }

        void pop_back() {
            if (empty()) throw std::out_of_range("Vector::pop_back - empty vector");
            --len;
        }

        void insert(size_t index, const T& value) {
            if (len >= cap) {
                reserve(cap == 0 ? DEFAULT_CAPACITY : cap * 2);
            }
            for (size_t i = len; i > index; --i) {
                data_[i] = data_[i - 1];
            }
            data_[index] = value;
            ++len;
        }

        void erase(size_t index) {
            for (size_t i = index; i < len - 1; ++i) {
                data_[i] = data_[i + 1];
            }
            --len;
        }

        void resize(size_t count, const T& value = T()) {
            if (count > len) {
                reserve(count);
                for (size_t i = len; i < count; ++i) {
                    push_back(value);
                }
            } else {
                len = count;
            }
        }

        void swap(Vector& other) noexcept {
            std::swap(data_, other.data_);
            std::swap(len, other.len);
            std::swap(cap, other.cap);
        }

        bool operator==(const Vector& other) const {
            if (len != other.len) return false;
            for (size_t i = 0; i < len; ++i) {
                if (data_[i] != other.data_[i]) {
                    return false;
                }
            }
            return true;
        }

        bool operator!=(const Vector& other) const {
            return !(*this == other);
        }

        bool operator<(const Vector& other) const {
            size_t min_size = std::min(len, other.len);
            for (size_t i = 0; i < min_size; ++i) {
                if (data_[i] < other.data_[i]) return true;
                if (other.data_[i] < data_[i]) return false;
            }
            return len < other.len;
        }

        bool operator<=(const Vector& other) const {
            return !(other < *this);
        }

        bool operator>(const Vector& other) const {
            return other < *this;
        }

        bool operator>=(const Vector& other) const {
            return !(*this < other);
        }

        std::strong_ordering operator<=>(const Vector& other) const {
            size_t min_size = std::min(len, other.len);
            for (size_t i = 0; i < min_size; ++i) {
                if (auto cmp = data_[i] <=> other.data_[i]; cmp != 0) {
                    return cmp;
                }
            }
            return len <=> other.len;
        }
    };

}

#endif // VECTOR_H