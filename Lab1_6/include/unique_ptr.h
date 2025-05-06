#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <cstddef>
#include <algorithm>
#include <memory>

namespace my_smart_ptr {
    template<typename T, class Deleter = std::default_delete<T>>
    class UniquePtr {
    private:
        Deleter deleter_ = Deleter();
        T *ptr_ = nullptr;

    public:
        UniquePtr() = default;

        explicit UniquePtr(T* ptr) : ptr_(ptr) {}

        UniquePtr(const UniquePtr&) = delete;

        UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
            deleter_ = other.deleter_;
        }

        ~UniquePtr()  {
            if (ptr_ != nullptr) {
                deleter_(ptr_);
            }
        }

        UniquePtr& operator=(const UniquePtr&) = delete;

        UniquePtr &operator=(UniquePtr &&other) noexcept {
            if (ptr_ != nullptr) {
                deleter_(ptr_);
            }
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
            deleter_ = other.deleter_;
            return *this;
        }

        T* get() const {
            return ptr_;
        }

        T& operator*() const {
            return *ptr_;
        }

        T* operator->() const {
            return ptr_;
        }

        explicit operator bool() const {
            return ptr_ != nullptr;
        }

        void reset(T* ptr = nullptr) {
            if (ptr_ != nullptr) {
                deleter_(ptr_);
            }
            ptr_ = ptr;
        }

        T* release() {
            T *tmp = ptr_;
            ptr_ = nullptr;
            return tmp;
        }

        void swap(UniquePtr& other) noexcept {
            std::swap(ptr_, other.ptr_);
            std::swap(deleter_, other.deleter_);
        }
    };
    template<typename T, class Deleter>
    class UniquePtr<T[], Deleter> {
    private:
        Deleter arr_deleter_ = Deleter();
        T *ptr_ = nullptr;
    public:
        UniquePtr() : ptr_(nullptr) {};

        explicit UniquePtr(T* ptr) : ptr_(ptr) {}

        UniquePtr(const UniquePtr&) = delete;

        UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
            other.ptr_ = nullptr;
            arr_deleter_ = other.arr_deleter_;
        }

        ~UniquePtr()  {
            if (ptr_ != nullptr) {
                arr_deleter_(ptr_);
            }
        }

        UniquePtr& operator=(const UniquePtr&) = delete;

        UniquePtr &operator=(UniquePtr &&other) noexcept {
            if (ptr_ != nullptr) {
                arr_deleter_(ptr_);
            }
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
            arr_deleter_ = other.arr_deleter_;
            return *this;
        }

        T& operator[](size_t index) const {
            return ptr_[index];
        }

        T* get() const noexcept {
            return ptr_;
        }

        T& operator*() const {
            return *ptr_;
        }

        T* operator->() const noexcept {
            return ptr_;
        }

        explicit operator bool() const noexcept {
            return ptr_ != nullptr;
        }

        void reset(T* ptr = nullptr) noexcept {
            if (ptr_ != nullptr) {
                arr_deleter_(ptr_);
            }
            ptr_ = ptr;
        }

        T* release() noexcept {
            T *tmp = ptr_;
            ptr_ = nullptr;
            return tmp;
        }

        void swap(UniquePtr& other) noexcept {
            std::swap(ptr_, other.ptr_);
            std::swap(arr_deleter_, other.arr_deleter_);
        }
    };
}

#endif