#ifndef MY_LIB_H
#define MY_LIB_H

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
        [[nodiscard]] virtual std::size_t size() const = 0;
        [[nodiscard]] virtual std::size_t max_size() const = 0;
    };

    template <typename T>
    class List : public Container<T> {
    private:
        class Node {
        protected:
            T data;
            Node* prev;
            Node* next;

            explicit Node(const T& val, Node* p = nullptr, Node* n = nullptr)
                : data(val), prev(p), next(n) {}

            friend class List;
        };

        Node* head;
        Node* tail;
        std::size_t cap;

    public:
        template <typename IterType>
        class ListIterator {
        private:
            friend class List;
            Node* current;

        public:

            explicit ListIterator(Node* node = nullptr) : current(node) {}

            IterType& operator*() const {
                if (!current) throw std::out_of_range("Dereferencing null iterator");
                return current->data;
            }
            IterType* operator->() const { return &current->data; }

            ListIterator operator++() {
                current = current->next;
                return *this;
            }

            ListIterator operator++(int) {
                ListIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            ListIterator operator--() {
                current = current->prev;
                return *this;
            }

            ListIterator operator--(int) {
                ListIterator tmp = *this;
                --(*this);
                return tmp;
            }

            bool operator==(const ListIterator& other) const {
                return current == other.current;
            }

            bool operator!=(const ListIterator& other) const {
                return !(*this == other);
            }

            ListIterator &next(std::size_t offset = 1) {
                for (std::size_t i = 0; i < offset; i++) {
                    if (current == nullptr) {
                        throw std::out_of_range("ListIterator::next. Offset is out of the collection.");
                    }
                    current = current->next;
                }
                return *this;
            }

        };

        template <typename IterType>
        class ListReverseIterator {
        private:
            friend class List;
            Node* current;
        public:

            explicit ListReverseIterator(Node* node = nullptr) : current(node) {}

            IterType& operator*() const {
                if (!current) throw std::out_of_range("Dereferencing null iterator");
                return current->data;
            }
            IterType* operator->() const { return &current->data; }

            ListReverseIterator operator++() {
                current = current->prev;
                return *this;
            }

            ListReverseIterator operator++(int) {
                ListReverseIterator tmp = *this;
                ++(*this);
                return tmp;
            }

            ListReverseIterator operator--() {
                current = current->next;
                return *this;
            }

            ListReverseIterator operator--(int) {
                ListReverseIterator tmp = *this;
                --(*this);
                return tmp;
            }

            bool operator==(const ListReverseIterator& other) const {
                return current == other.current;
            }

            bool operator!=(const ListReverseIterator& other) const {
                return !(*this == other);
            }
        };

        using iterator = ListIterator<T>;
        using const_iterator = ListIterator<const T>;
        using reverse_iterator = ListReverseIterator<T>;
        using const_reverse_iterator = ListReverseIterator<const T>;

        List() : head(nullptr), tail(nullptr), cap(0) {}

        List(std::initializer_list<T> init) : List() {
            for (const auto& val : init) {
                push_back(val);
            }
        }

        List(const List& other) : List() {
            for (const auto& val : other) {
                push_back(val);
            }
        }

        List(List&& other) noexcept
            : head(other.head), tail(other.tail), cap(other.cap) {
            other.head = nullptr;
            other.tail = nullptr;
            other.cap = 0;
        }

        ~List() override {
            List<T>::clear();
        }

        List& operator=(const List& other) {
            if (this != &other) {
                clear();
                for (const auto& val : other) {
                    push_back(val);
                }
            }
            return *this;
        }

        List& operator=(List&& other) noexcept {
            if (this != &other) {
                clear();
                head = other.head;
                tail = other.tail;
                cap = other.cap;
                other.head = nullptr;
                other.tail = nullptr;
                other.cap = 0;
            }
            return *this;
        }

        virtual iterator begin() const { return iterator(head); }
        virtual const_iterator cbegin() const { return const_iterator(head); }

        virtual iterator end() const { return iterator(nullptr); }
        virtual const_iterator cend() const { return const_iterator(nullptr); }

        virtual reverse_iterator rbegin() const { return reverse_iterator(tail); }
        virtual const_reverse_iterator crbegin() const { return const_reverse_iterator(tail); }

        virtual reverse_iterator rend() const { return reverse_iterator(nullptr); }
        virtual const_reverse_iterator crend() const { return const_reverse_iterator(nullptr); }

        [[nodiscard]] bool empty() const override { return cap == 0; }
        [[nodiscard]] std::size_t size() const override { return cap; }
        [[nodiscard]] std::size_t max_size() const override { return cap; }

        virtual T& front() {
            if (empty() || !head) throw std::out_of_range("List is empty");
            return head->data;
        }

        virtual T& back() {
            if (empty()) throw std::out_of_range("List is empty");
            return tail->data;
        }

        virtual void clear() {
            while (head) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            tail = nullptr;
            cap = 0;
        }

        virtual iterator insert(iterator pos, const T& value) {
            if (pos == end()) {
                push_back(value);
                return iterator(tail);
            }
            else if (pos == begin()) {
                push_front(value);
                return begin();
            }
            else {
                Node* newNode = new Node(value, pos.current->prev, pos.current);
                pos.current->prev->next = newNode;
                pos.current->prev = newNode;
                ++cap;
                return iterator(newNode);
            }
        }

        virtual iterator erase(iterator pos) {
            if (pos == end() || empty()) {
                return end();
            }

            Node* toDelete = pos.current;
            iterator nextIter(toDelete->next);

            if (toDelete->prev) {
                toDelete->prev->next = toDelete->next;
            }
            else {
                head = toDelete->next;
            }

            if (toDelete->next) {
                toDelete->next->prev = toDelete->prev;
            }
            else {
                tail = toDelete->prev;
            }

            delete toDelete;
            --cap;
            return nextIter;
        }


        virtual void push_back(const T& value) {
            Node* newNode = new Node(value, tail, nullptr);
            if (tail) {
                tail->next = newNode;
            } else {
                head = newNode;
            }
            tail = newNode;
            ++cap;
        }

        virtual void pop_back() {
            if (empty()) throw std::out_of_range("List is empty");

            Node* to_delete = tail;
            tail = tail->prev;
            if (tail) {
                tail->next = nullptr;
            } else {
                head = nullptr;
            }
            delete to_delete;
            --cap;
        }

        virtual void push_front(const T& value) {
            Node* new_node = new Node(value, nullptr, head);
            if (head) {
                head->prev = new_node;
            } else {
                tail = new_node;
            }
            head = new_node;
            ++cap;
        }

        virtual void pop_front() {
            if (empty()) throw std::out_of_range("List is empty");

            Node* to_delete = head;
            head = head->next;
            if (head) {
                head->prev = nullptr;
            } else {
                tail = nullptr;
            }
            delete to_delete;
            --cap;
        }

        virtual void resize(std::size_t count) {
            while (cap > count) {
                pop_back();
            }
            while (cap < count) {
                push_back(T());
            }
        }

        virtual void swap(List& other) noexcept {
            std::swap(head, other.head);
            std::swap(tail, other.tail);
            std::swap(cap, other.cap);
        }

        bool operator==(const List& other) const {
            if (cap != other.cap) return false;

            auto it1 = begin();
            auto it2 = other.begin();
            while (it1 != end()) {
                if (*it1 != *it2) return false;
                ++it1;
                ++it2;
            }
            return true;
        }

        bool operator!=(const List& other) const {
            return !(*this == other);
        }

        std::strong_ordering operator<=>(const List& other) const {
            auto it1 = begin();
            auto it2 = other.begin();

            while (it1 != end() && it2 != other.end()) {
                if (auto cmp = *it1 <=> *it2; cmp != 0) {
                    return cmp;
                }
                ++it1;
                ++it2;
            }

            return size() <=> other.size();
        }

    };
}

#endif