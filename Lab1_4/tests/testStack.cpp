#include <gtest/gtest.h>
#include "Stack.h"
#include <algorithm>
#include <string>
#include <vector>

using namespace my_cont;

class StackTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    Stack<int> emptyStack;
    Stack<int> stackWithValues{1, 2, 3, 4, 5};
};

// Конструкторы и операторы присваивания
TEST_F(StackTest, DefaultConstructor) {
    EXPECT_TRUE(emptyStack.empty());
    EXPECT_EQ(emptyStack.size(), 0);
}

TEST_F(StackTest, InitializerListConstructor) {
    EXPECT_FALSE(stackWithValues.empty());
    EXPECT_EQ(stackWithValues.size(), 5);
    EXPECT_EQ(stackWithValues.top(), 5);
}

TEST_F(StackTest, CopyConstructor) {
    Stack<int> copy(stackWithValues);
    EXPECT_EQ(copy.size(), stackWithValues.size());
    EXPECT_EQ(copy.top(), stackWithValues.top());
}

TEST_F(StackTest, MoveConstructor) {
    Stack<int> original{10, 20, 30};
    Stack<int> moved(std::move(original));
    EXPECT_TRUE(original.empty());
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.top(), 30);
}

TEST_F(StackTest, CopyAssignment) {
    Stack<int> copy;
    copy = stackWithValues;
    EXPECT_EQ(copy.size(), stackWithValues.size());
    EXPECT_EQ(copy.top(), stackWithValues.top());
}

TEST_F(StackTest, MoveAssignment) {
    Stack<int> original{10, 20, 30};
    Stack<int> moved;
    moved = std::move(original);
    EXPECT_TRUE(original.empty());
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.top(), 30);
}

// Основные операции
TEST_F(StackTest, Top) {
    EXPECT_EQ(stackWithValues.top(), 5);

    stackWithValues.push(6);
    EXPECT_EQ(stackWithValues.top(), 6);
}

TEST_F(StackTest, TopThrowWhenEmpty) {
    EXPECT_THROW(emptyStack.top(), std::out_of_range);
}

TEST_F(StackTest, PushAndPop) {
    stackWithValues.push(6);
    EXPECT_EQ(stackWithValues.top(), 6);
    EXPECT_EQ(stackWithValues.size(), 6);

    stackWithValues.pop();
    EXPECT_EQ(stackWithValues.top(), 5);
    EXPECT_EQ(stackWithValues.size(), 5);
}

TEST_F(StackTest, PopThrowWhenEmpty) {
    EXPECT_THROW(emptyStack.pop(), std::out_of_range);
}

TEST_F(StackTest, EmptyAndSize) {
    EXPECT_TRUE(emptyStack.empty());
    EXPECT_EQ(emptyStack.size(), 0);

    EXPECT_FALSE(stackWithValues.empty());
    EXPECT_EQ(stackWithValues.size(), 5);
}

TEST_F(StackTest, Swap) {
    Stack<int> other{10, 20, 30};
    stackWithValues.swap(other);

    EXPECT_EQ(stackWithValues.size(), 3);
    EXPECT_EQ(stackWithValues.top(), 30);

    EXPECT_EQ(other.size(), 5);
    EXPECT_EQ(other.top(), 5);
}

// Операторы сравнения
TEST_F(StackTest, ComparisonOperators) {
    Stack<int> same{1, 2, 3, 4, 5};
    Stack<int> smaller{1, 2, 3};

    EXPECT_TRUE(stackWithValues == same);
    EXPECT_FALSE(stackWithValues == smaller);

    EXPECT_TRUE(stackWithValues != smaller);
    EXPECT_FALSE(stackWithValues != same);

    EXPECT_TRUE(smaller < stackWithValues);
    EXPECT_FALSE(stackWithValues < smaller);

    EXPECT_TRUE(smaller <= stackWithValues);
    EXPECT_TRUE(stackWithValues >= smaller);

    EXPECT_EQ((stackWithValues <=> same), std::strong_ordering::equal);
    EXPECT_EQ((stackWithValues <=> smaller), std::strong_ordering::greater);
}

// Краевые случаи
TEST_F(StackTest, SingleElementStack) {
    Stack<int> single{42};

    EXPECT_EQ(single.size(), 1);
    EXPECT_EQ(single.top(), 42);

    single.pop();
    EXPECT_TRUE(single.empty());

    single.push(100);
    EXPECT_EQ(single.size(), 1);
    EXPECT_EQ(single.top(), 100);
}

TEST_F(StackTest, LargeStack) {
    const int largeSize = 10000;
    Stack<int> largeStack;

    for (int i = 0; i < largeSize; ++i) {
        largeStack.push(i);
    }

    EXPECT_EQ(largeStack.size(), largeSize);
    EXPECT_EQ(largeStack.top(), largeSize - 1);
}

TEST_F(StackTest, DifferentContainerType) {
    // Тест с другим контейнером (Deque по умолчанию)
    Stack<int, Deque<int>> dequeStack;
    dequeStack.push(1);
    dequeStack.push(2);
    EXPECT_EQ(dequeStack.top(), 2);
    EXPECT_EQ(dequeStack.size(), 2);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}