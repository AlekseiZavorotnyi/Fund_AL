#include <gtest/gtest.h>
#include "stack.h"
#include <algorithm>
#include <string>

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

    stackWithValues.pop();
    EXPECT_EQ(copy.size(), 5);
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

    copy = copy;
    EXPECT_EQ(copy.size(), 5);
}

TEST_F(StackTest, MoveAssignment) {
    Stack<int> original{10, 20, 30};
    Stack<int> moved;
    moved = std::move(original);

    EXPECT_TRUE(original.empty());
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.top(), 30);
}

TEST_F(StackTest, Top) {
    EXPECT_EQ(stackWithValues.top(), 5);

    stackWithValues.push(0);
    EXPECT_EQ(stackWithValues.top(), 0);

}

TEST_F(StackTest, TopThrowWhenEmpty) {
    EXPECT_THROW(emptyStack.top(), std::out_of_range);
}

TEST_F(StackTest, EmptyAndSize) {
    EXPECT_TRUE(emptyStack.empty());
    EXPECT_EQ(emptyStack.size(), 0);

    EXPECT_FALSE(stackWithValues.empty());
    EXPECT_EQ(stackWithValues.size(), 5);
}

TEST_F(StackTest, PushPop) {
    stackWithValues.push(0);
    EXPECT_EQ(stackWithValues.top(), 0);
    EXPECT_EQ(stackWithValues.size(), 6);

    stackWithValues.pop();
    EXPECT_EQ(stackWithValues.top(), 5);
    EXPECT_EQ(stackWithValues.size(), 5);
}

TEST_F(StackTest, PopEmptyThrows) {
    EXPECT_THROW(emptyStack.pop(), std::out_of_range);
}

TEST_F(StackTest, Swap) {
    Stack<int> other{10, 20, 30};
    stackWithValues.swap(other);

    EXPECT_EQ(stackWithValues.size(), 3);
    EXPECT_EQ(stackWithValues.top(), 30);

    EXPECT_EQ(other.size(), 5);
    EXPECT_EQ(other.top(), 5);
}

TEST_F(StackTest, ComparisonOperators) {
    Stack<int> same{1, 2, 3, 4, 5};
    Stack<int> smaller{1, 2, 3};
    Stack<int> smaller2{1, 2, 2};
    Stack<int> greater{1, 2, 3, 4, 5, 6};
    Stack<int> greater2{1, 2, 4};
    Stack<int> different{5, 4, 3, 2, 1};


    EXPECT_TRUE(stackWithValues == same);
    EXPECT_FALSE(stackWithValues == smaller);

    EXPECT_TRUE(stackWithValues != smaller);
    EXPECT_FALSE(stackWithValues != same);

    EXPECT_TRUE(smaller < stackWithValues);
    EXPECT_TRUE(smaller2 < stackWithValues);
    EXPECT_FALSE(stackWithValues < smaller);
    EXPECT_FALSE(stackWithValues < same);

    EXPECT_TRUE(smaller <= stackWithValues);
    EXPECT_TRUE(stackWithValues <= same);
    EXPECT_FALSE(stackWithValues <= smaller2);

    EXPECT_TRUE(stackWithValues > smaller);
    EXPECT_TRUE(greater > stackWithValues);
    EXPECT_TRUE(greater2 > stackWithValues);
    EXPECT_FALSE(smaller > stackWithValues);
    EXPECT_FALSE(stackWithValues > same);

    EXPECT_TRUE(stackWithValues >= smaller);
    EXPECT_TRUE(stackWithValues >= same);
    EXPECT_FALSE(smaller >= stackWithValues);

    EXPECT_EQ((stackWithValues <=> same), std::strong_ordering::equal);
    EXPECT_EQ((stackWithValues <=> smaller), std::strong_ordering::greater);
    EXPECT_EQ((smaller <=> stackWithValues), std::strong_ordering::less);
    EXPECT_EQ((stackWithValues <=> greater), std::strong_ordering::less);
    EXPECT_EQ((greater <=> stackWithValues), std::strong_ordering::greater);
    EXPECT_EQ((stackWithValues <=> smaller2), std::strong_ordering::greater);
    EXPECT_EQ((smaller2 <=> stackWithValues), std::strong_ordering::less);
}

TEST_F(StackTest, SingleElementList) {
    Stack<int> single{42};

    EXPECT_EQ(single.size(), 1);
    EXPECT_EQ(single.top(), 42);

    single.pop();
    EXPECT_TRUE(single.empty());

    single.push(100);
    EXPECT_EQ(single.size(), 1);
    EXPECT_EQ(single.top(), 100);

    single.pop();
    EXPECT_TRUE(single.empty());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}