#ifndef DEQUE_H
#define DEQUE_H

#include "../../Lab1_2/include/list.h"

namespace my_cont {
    template<class T>
    class Deque : public List<T> {
    private:
        List<T> list;

    public:
        Deque() {
            list = List<T>();
        }

        Deque(std::initializer_list<T> init) {
            list = List<T>(init);
        }

        Deque(const Deque &other) : List<T>(other.list) {
            list = other.list;
        }

        Deque(Deque &&other) noexcept {
            list = other.list;
            other.list.clear();
        }

        ~Deque() override {
            list.clear();
        }

        Deque &operator=(const Deque &other) {
            if (this != &other) {
                list.clear();
                list = other.list;
            }
            return *this;
        }

        Deque &operator=(Deque &&other) noexcept {
            if (this != &other) {
                list.clear();
                list = other.list;
                other.list.clear();
            }
            return *this;
        }

        T &at(std::size_t pos) {
            if (pos >= list.size()) {
                throw std::out_of_range("Deque::at. Pos is unreal.");
            }
            auto it = list.begin().next(pos);
            return *it;
        }

        T &operator[](std::size_t pos) {
            if (pos >= list.size()) {
                throw std::out_of_range("Deque::[]. Pos is unreal.");
            }
            auto it = list.begin();
            while(pos > 0) {
                ++it;
                pos--;
            }
            return *it;
        }

        T &front() override {
            return list.front();
        }

        T &back() override {
            return list.back();
        }

        using Iterator = List<T>::iterator;
        using ConstIterator = List<T>::const_iterator;
        using ReverseIterator = List<T>::reverse_iterator;
        using ConstReverseIterator = List<T>::const_reverse_iterator;

        Iterator begin() const override {
            return list.begin();
        }

        Iterator end() const override {
            return list.end();
        }

        ConstIterator cbegin() const override {
            return list.cbegin();
        }

        ConstIterator cend() const override {
            return list.cend();
        }

        ReverseIterator rbegin() const override {
            return list.rbegin();
        }

        ReverseIterator rend() const override {
            return list.rend();
        }

        ConstReverseIterator crbegin() const override {
            return list.crbegin();
        }

        ConstReverseIterator crend() const override {
            return list.crend();
        }

        [[nodiscard]] bool empty() const override {
            return list.empty();
        }

        [[nodiscard]] std::size_t size() const override {
            return list.size();
        }

        [[nodiscard]] std::size_t max_size() const override {
            return list.max_size();
        }

        void clear() override {
            list.clear();
        }

        Iterator insert(Iterator posIter, const T& val) override {
            return list.insert(posIter, val);
        }

        Iterator erase(Iterator posIter) override {
            return list.erase(posIter);
        }

        void push_back(const T& val) override {
            list.push_back(val);
        }

        void pop_back() override {
            list.pop_back();
        }

        void push_front(const T& val) override {
            list.push_front(val);
        }

        void pop_front() override {
            list.pop_front();
        }

        void resize(std::size_t newSize, T val) override {
            list.resize(newSize, val);
        }

        void swap(Deque &other) noexcept {
            list.swap(other.list);
        }

        bool operator==(const Deque &other) const {
            return list == other.list;
        }

        bool operator!=(const Deque &other) const {
            return list != other.list;
        }

        bool operator<(const Deque &other) const {
            return list < other.list;
        }

        bool operator<=(const Deque &other) const {
            return list <= other.list;
        }

        bool operator>(const Deque &other) const {
            return list > other.list;
        }

        bool operator>=(const Deque &other) const {
            return list >= other.list;
        }

        std::strong_ordering operator<=>(const Deque &other) const {
            return list <=> other.list;
        }
    };
};

#endif //DEQUE_H