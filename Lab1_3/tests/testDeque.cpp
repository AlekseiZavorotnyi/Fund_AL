#include <gtest/gtest.h>
#include "Deque.h"
#include <algorithm>
#include <string>

using namespace my_cont;

class DequeTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    Deque<int> emptyDeque;
    Deque<int> dequeWithValues{1, 2, 3, 4, 5};
};

// Конструкторы
TEST_F(DequeTest, DefaultConstructor) {
    EXPECT_TRUE(emptyDeque.empty());
    EXPECT_EQ(emptyDeque.size(), 0);
}

TEST_F(DequeTest, InitializerListConstructor) {
    EXPECT_FALSE(dequeWithValues.empty());
    EXPECT_EQ(dequeWithValues.size(), 5);
    EXPECT_EQ(dequeWithValues.front(), 1);
    EXPECT_EQ(dequeWithValues.back(), 5);
}

TEST_F(DequeTest, CopyConstructor) {
    Deque<int> copy(dequeWithValues);
    EXPECT_EQ(copy.size(), dequeWithValues.size());
    EXPECT_EQ(copy.front(), dequeWithValues.front());
    EXPECT_EQ(copy.back(), dequeWithValues.back());
}

TEST_F(DequeTest, MoveConstructor) {
    Deque<int> original{10, 20, 30};
    Deque<int> moved(std::move(original));
    EXPECT_TRUE(original.empty());
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 10);
    EXPECT_EQ(moved.back(), 30);
}

// Операторы присваивания
TEST_F(DequeTest, CopyAssignment) {
    Deque<int> copy;
    copy = dequeWithValues;
    EXPECT_EQ(copy.size(), dequeWithValues.size());
    EXPECT_EQ(copy.front(), dequeWithValues.front());
    EXPECT_EQ(copy.back(), dequeWithValues.back());
}

TEST_F(DequeTest, MoveAssignment) {
    Deque<int> original{10, 20, 30};
    Deque<int> moved;
    moved = std::move(original);
    EXPECT_TRUE(original.empty());
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 10);
    EXPECT_EQ(moved.back(), 30);
}

// Доступ к элементам
TEST_F(DequeTest, FrontAndBack) {
    EXPECT_EQ(dequeWithValues.front(), 1);
    EXPECT_EQ(dequeWithValues.back(), 5);

    dequeWithValues.push_front(0);
    EXPECT_EQ(dequeWithValues.front(), 0);

    dequeWithValues.push_back(6);
    EXPECT_EQ(dequeWithValues.back(), 6);
}

TEST_F(DequeTest, FrontBackThrowWhenEmpty) {
    EXPECT_THROW(emptyDeque.front(), std::out_of_range);
    EXPECT_THROW(emptyDeque.back(), std::out_of_range);
}

TEST_F(DequeTest, AtAndOperatorBrackets) {
    EXPECT_EQ(dequeWithValues.at(0), 1);
    EXPECT_EQ(dequeWithValues[2], 3);
    EXPECT_EQ(dequeWithValues.at(4), 5);

    EXPECT_THROW(dequeWithValues.at(5), std::out_of_range);
    EXPECT_THROW(dequeWithValues[5], std::out_of_range);
}

// Итераторы
TEST_F(DequeTest, Iterators) {
    int expected = 1;
    for (auto it = dequeWithValues.begin(); it != dequeWithValues.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }

    expected = 1;
    for (auto it = dequeWithValues.cbegin(); it != dequeWithValues.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }

    expected = 5;
    for (auto it = dequeWithValues.rbegin(); it != dequeWithValues.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }

    expected = 5;
    for (auto it = dequeWithValues.crbegin(); it != dequeWithValues.crend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
}

// Модификаторы
TEST_F(DequeTest, PushPopFrontBack) {
    dequeWithValues.push_front(0);
    EXPECT_EQ(dequeWithValues.front(), 0);
    EXPECT_EQ(dequeWithValues.size(), 6);

    dequeWithValues.pop_front();
    EXPECT_EQ(dequeWithValues.front(), 1);
    EXPECT_EQ(dequeWithValues.size(), 5);

    dequeWithValues.push_back(6);
    EXPECT_EQ(dequeWithValues.back(), 6);
    EXPECT_EQ(dequeWithValues.size(), 6);

    dequeWithValues.pop_back();
    EXPECT_EQ(dequeWithValues.back(), 5);
    EXPECT_EQ(dequeWithValues.size(), 5);
}

TEST_F(DequeTest, PopEmptyThrows) {
    EXPECT_THROW(emptyDeque.pop_front(), std::out_of_range);
    EXPECT_THROW(emptyDeque.pop_back(), std::out_of_range);
}

TEST_F(DequeTest, Insert) {
    auto it = dequeWithValues.begin();
    ++it; // указывает на 2
    dequeWithValues.insert(it, 42);
    EXPECT_EQ(dequeWithValues[1], 42);
    EXPECT_EQ(dequeWithValues.size(), 6);
}

TEST_F(DequeTest, Erase) {
    auto it = dequeWithValues.begin();
    ++it; // указывает на 2
    dequeWithValues.erase(it);
    EXPECT_EQ(dequeWithValues[1], 3);
    EXPECT_EQ(dequeWithValues.size(), 4);
}

TEST_F(DequeTest, Clear) {
    EXPECT_FALSE(dequeWithValues.empty());
    dequeWithValues.clear();
    EXPECT_TRUE(dequeWithValues.empty());
    EXPECT_EQ(dequeWithValues.size(), 0);
}

TEST_F(DequeTest, Resize) {
    dequeWithValues.resize(3, 0);
    EXPECT_EQ(dequeWithValues.size(), 3);
    EXPECT_EQ(dequeWithValues.back(), 3);

    dequeWithValues.resize(5, 0);
    EXPECT_EQ(dequeWithValues.size(), 5);
    EXPECT_EQ(dequeWithValues.back(), 0);
}

TEST_F(DequeTest, Swap) {
    Deque<int> other{10, 20, 30};
    dequeWithValues.swap(other);

    EXPECT_EQ(dequeWithValues.size(), 3);
    EXPECT_EQ(dequeWithValues.front(), 10);
    EXPECT_EQ(dequeWithValues.back(), 30);

    EXPECT_EQ(other.size(), 5);
    EXPECT_EQ(other.front(), 1);
    EXPECT_EQ(other.back(), 5);
}

// Операторы сравнения
TEST_F(DequeTest, ComparisonOperators) {
    Deque<int> same{1, 2, 3, 4, 5};
    Deque<int> smaller{1, 2, 3};

    EXPECT_TRUE(dequeWithValues == same);
    EXPECT_FALSE(dequeWithValues == smaller);

    EXPECT_TRUE(dequeWithValues != smaller);
    EXPECT_FALSE(dequeWithValues != same);

    EXPECT_TRUE(smaller < dequeWithValues);
    EXPECT_FALSE(dequeWithValues < smaller);

    EXPECT_TRUE(smaller <= dequeWithValues);
    EXPECT_TRUE(dequeWithValues >= smaller);

    EXPECT_EQ((dequeWithValues <=> same), std::strong_ordering::equal);
    EXPECT_EQ((dequeWithValues <=> smaller), std::strong_ordering::greater);
}

// Краевые случаи
TEST_F(DequeTest, SingleElementDeque) {
    Deque<int> single{42};

    EXPECT_EQ(single.size(), 1);
    EXPECT_EQ(single.front(), 42);
    EXPECT_EQ(single.back(), 42);

    single.pop_front();
    EXPECT_TRUE(single.empty());

    single.push_back(100);
    EXPECT_EQ(single.size(), 1);
    EXPECT_EQ(single.front(), 100);
    EXPECT_EQ(single.back(), 100);
}

TEST_F(DequeTest, LargeDeque) {
    const int largeSize = 10000;
    Deque<int> largeDeque;

    for (int i = 0; i < largeSize; ++i) {
        largeDeque.push_back(i);
    }

    EXPECT_EQ(largeDeque.size(), largeSize);
    EXPECT_EQ(largeDeque.front(), 0);
    EXPECT_EQ(largeDeque.back(), largeSize - 1);

    int count = 0;
    for (auto it = largeDeque.begin(); it != largeDeque.end(); ++it) {
        EXPECT_EQ(*it, count++);
    }

    largeDeque.clear();
    EXPECT_TRUE(largeDeque.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}