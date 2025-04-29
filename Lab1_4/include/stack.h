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

        Stack(std::initializer_list<T> init) {
            for (const auto& val : init) {
                push(val);
            }
        }

        Stack(const Stack& other) : stack(other.stack) {}

        Stack(Stack&& other) noexcept : stack(std::move(other.stack)){}

        ~Stack() {
            stack.clear();
        }

        Stack& operator=(const Stack& other) {
            if (this != &other) {
                stack.clear();
                stack = Container(other.stack);
            }
            return *this;
        }

        Stack& operator=(Stack&& other) noexcept {
            if (this != &other) {
                stack = other.stack;
                other.stack = Container();
            }
            return *this;
        }

        T &top() {
            return stack.front();
        }

        [[nodiscard]] bool empty() const { return stack.size() == 0; }
        [[nodiscard]] std::size_t size() const { return stack.size(); }

        void push(const T& value) {
            stack.push_front(value);
        }

        void pop() {
            stack.pop_front();
        }

        void swap(Stack& other) noexcept {
            stack.swap(other.stack);
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