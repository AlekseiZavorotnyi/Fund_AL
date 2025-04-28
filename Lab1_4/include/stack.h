#ifndef STACK_H
#define STACK_H

#include "../../Lab1_3/include/deque.h"

namespace my_cont {
    template <typename T, class Container = Deque<T>>
    class Stack {
    private:
        Container stack;

    public:
        Stack() : stack() {}

        Stack(std::initializer_list<T> init) : Stack() {
            for (const auto& val : init) {
                push(val);
            }
        }

        Stack(const Stack& other) : Stack() {
            stack(other.stack);
        }

        Stack(Stack&& other) noexcept{
            stack(other.stack);
        }

        ~Stack() {
            stack.clear();
        }

        Stack& operator=(const Stack& other) {
            stack = other.stack;
            return *this;
        }

        Stack& operator=(Stack&& other) noexcept {
            stack = other.stack;
            return *this;
        }

        [[nodiscard]] bool empty() const { return stack.cap == 0; }
        [[nodiscard]] std::size_t size() const { return stack.cap; }

        void push(const T& value) {
            stack.push_front(value);
        }

        void pop() {
            stack.pop_front();
        }

        void swap(Stack& other) noexcept {
            stack.swap(other);
        }

        bool operator==(const Stack &other) const {
            return stack == other.stack;
        }

        bool operator!=(const Stack &other) const {
            return stack != other.stack;
        }

        bool operator<(const Stack &other) const {
            return stack < other.stack;
        }

        bool operator<=(const Stack &other) const {
            return stack <= other.stack;
        }

        bool operator>(const Stack &other) const {
            return stack > other.stack;
        }

        bool operator>=(const Stack &other) const {
            return stack >= other.stack;
        }

        std::strong_ordering operator<=>(const Stack &other) const {
            return stack <=> other.stack;
        }

    };
}

#endif