#include <gtest/gtest.h>
#include "deque.h"
#include <algorithm>
#include <string>

using namespace my_cont;

class ListTest : public ::testing::Test {
protected:
    void SetUp() override {
    }

    void TearDown() override {
    }

    List<int> emptyList;
    List<int> listWithValues{1, 2, 3, 4, 5};
};

TEST_F(ListTest, DefaultConstructor) {
    EXPECT_TRUE(emptyList.empty());
    EXPECT_EQ(emptyList.size(), 0);
}

TEST_F(ListTest, InitializerListConstructor) {
    EXPECT_FALSE(listWithValues.empty());
    EXPECT_EQ(listWithValues.size(), 5);

    EXPECT_EQ(listWithValues.front(), 1);
    EXPECT_EQ(listWithValues.back(), 5);
}

TEST_F(ListTest, CopyConstructor) {
    List<int> copy(listWithValues);

    EXPECT_EQ(copy.size(), listWithValues.size());
    EXPECT_EQ(copy.front(), listWithValues.front());
    EXPECT_EQ(copy.back(), listWithValues.back());

    listWithValues.pop_front();
    EXPECT_NE(copy.front(), listWithValues.front());
    EXPECT_EQ(copy.size(), 5);
}

TEST_F(ListTest, MoveConstructor) {
    List<int> original{10, 20, 30};
    List<int> moved(std::move(original));

    EXPECT_TRUE(original.empty());
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 10);
    EXPECT_EQ(moved.back(), 30);
}

TEST_F(ListTest, CopyAssignment) {
    List<int> copy;
    copy = listWithValues;

    EXPECT_EQ(copy.size(), listWithValues.size());
    EXPECT_EQ(copy.front(), listWithValues.front());
    EXPECT_EQ(copy.back(), listWithValues.back());

    copy = copy;
    EXPECT_EQ(copy.size(), 5);
}

TEST_F(ListTest, MoveAssignment) {
    List<int> original{10, 20, 30};
    List<int> moved;
    moved = std::move(original);

    EXPECT_TRUE(original.empty());
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved.front(), 10);
    EXPECT_EQ(moved.back(), 30);
}

TEST_F(ListTest, FrontAndBack) {
    EXPECT_EQ(listWithValues.front(), 1);
    EXPECT_EQ(listWithValues.back(), 5);

    listWithValues.push_front(0);
    EXPECT_EQ(listWithValues.front(), 0);

    listWithValues.push_back(6);
    EXPECT_EQ(listWithValues.back(), 6);
}

TEST_F(ListTest, FrontBackThrowWhenEmpty) {
    EXPECT_THROW(emptyList.front(), std::out_of_range);
    EXPECT_THROW(emptyList.back(), std::out_of_range);
}

TEST_F(ListTest, Iterators) {
    int expected = 1;
    for (auto it = listWithValues.begin(); it != listWithValues.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }

    expected = 1;
    for (auto it = listWithValues.cbegin(); it != listWithValues.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }

    expected = 5;
    for (auto it = listWithValues.rbegin(); it != listWithValues.rend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }

    expected = 5;
    for (auto it = listWithValues.crbegin(); it != listWithValues.crend(); ++it) {
        EXPECT_EQ(*it, expected--);
    }
}

TEST_F(ListTest, IteratorOperations) {
    auto it1 = listWithValues.begin();
    auto it2 = it1;
    ++it2;

    EXPECT_EQ(*it1, 1);
    EXPECT_EQ(*it2, 2);

    --it2;
    EXPECT_EQ(it1, it2);

    it1++;
    EXPECT_NE(it1, it2);

    it1--;
    EXPECT_EQ(it1, it2);
}

TEST_F(ListTest, EmptyAndSize) {
    EXPECT_TRUE(emptyList.empty());
    EXPECT_EQ(emptyList.size(), 0);

    EXPECT_FALSE(listWithValues.empty());
    EXPECT_EQ(listWithValues.size(), 5);
}

TEST_F(ListTest, PushPopFrontBack) {
    listWithValues.push_front(0);
    EXPECT_EQ(listWithValues.front(), 0);
    EXPECT_EQ(listWithValues.size(), 6);

    listWithValues.pop_front();
    EXPECT_EQ(listWithValues.front(), 1);
    EXPECT_EQ(listWithValues.size(), 5);

    listWithValues.push_back(6);
    EXPECT_EQ(listWithValues.back(), 6);
    EXPECT_EQ(listWithValues.size(), 6);

    listWithValues.pop_back();
    EXPECT_EQ(listWithValues.back(), 5);
    EXPECT_EQ(listWithValues.size(), 5);
}

TEST_F(ListTest, PopEmptyThrows) {
    EXPECT_THROW(emptyList.pop_front(), std::out_of_range);
    EXPECT_THROW(emptyList.pop_back(), std::out_of_range);
}

TEST_F(ListTest, Insert) {
    auto it = listWithValues.begin().next();
    listWithValues.insert(it, 42);
    auto itCheck = listWithValues.begin();
    ++itCheck;
    EXPECT_EQ(*itCheck, 42);
    EXPECT_EQ(listWithValues.size(), 6);
}

TEST_F(ListTest, Erase) {
    auto it = listWithValues.begin().next();
    listWithValues.erase(it);
    auto itCheck = ++listWithValues.begin();
    EXPECT_EQ(*itCheck, 3);
    EXPECT_EQ(listWithValues.size(), 4);

    List<int> l = {1, 2, 3, 4, 5};
    l.erase(l.begin());
    EXPECT_EQ(l.size(), 4);
    EXPECT_EQ(l.front(), 2);

    l.erase(l.begin().next(3));
    EXPECT_EQ(l.back(), 4);
    EXPECT_EQ(l.size(), 3);
}


TEST_F(ListTest, Clear) {
    EXPECT_FALSE(listWithValues.empty());
    listWithValues.clear();
    EXPECT_TRUE(listWithValues.empty());
    EXPECT_EQ(listWithValues.size(), 0);
}

TEST_F(ListTest, Resize) {
    listWithValues.resize(3);
    EXPECT_EQ(listWithValues.size(), 3);
    EXPECT_EQ(listWithValues.back(), 3);

    listWithValues.resize(5);
    EXPECT_EQ(listWithValues.size(), 5);
    EXPECT_EQ(listWithValues.back(), 0);
}

TEST_F(ListTest, Swap) {
    List<int> other{10, 20, 30};
    listWithValues.swap(other);

    EXPECT_EQ(listWithValues.size(), 3);
    EXPECT_EQ(listWithValues.front(), 10);
    EXPECT_EQ(listWithValues.back(), 30);

    EXPECT_EQ(other.size(), 5);
    EXPECT_EQ(other.front(), 1);
    EXPECT_EQ(other.back(), 5);
}

TEST_F(ListTest, ComparisonOperators) {
    List<int> same{1, 2, 3, 4, 5};
    List<int> smaller{1, 2, 3};
    List<int> smaller2{1, 2, 2};
    List<int> greater{1, 2, 3, 4, 5, 6};
    List<int> greater2{1, 2, 4};
    List<int> different{5, 4, 3, 2, 1};


    EXPECT_TRUE(listWithValues == same);
    EXPECT_FALSE(listWithValues == smaller);

    EXPECT_TRUE(listWithValues != smaller);
    EXPECT_FALSE(listWithValues != same);

    EXPECT_TRUE(smaller < listWithValues);
    EXPECT_TRUE(smaller2 < listWithValues);
    EXPECT_FALSE(listWithValues < smaller);
    EXPECT_FALSE(listWithValues < same);

    EXPECT_TRUE(smaller <= listWithValues);
    EXPECT_TRUE(listWithValues <= same);
    EXPECT_FALSE(listWithValues <= smaller2);

    EXPECT_TRUE(listWithValues > smaller);
    EXPECT_TRUE(greater > listWithValues);
    EXPECT_TRUE(greater2 > listWithValues);
    EXPECT_FALSE(smaller > listWithValues);
    EXPECT_FALSE(listWithValues > same);

    EXPECT_TRUE(listWithValues >= smaller);
    EXPECT_TRUE(listWithValues >= same);
    EXPECT_FALSE(smaller >= listWithValues);

    EXPECT_EQ((listWithValues <=> same), std::strong_ordering::equal);
    EXPECT_EQ((listWithValues <=> smaller), std::strong_ordering::greater);
    EXPECT_EQ((smaller <=> listWithValues), std::strong_ordering::less);
    EXPECT_EQ((listWithValues <=> greater), std::strong_ordering::less);
    EXPECT_EQ((greater <=> listWithValues), std::strong_ordering::greater);
    EXPECT_EQ((listWithValues <=> smaller2), std::strong_ordering::greater);
    EXPECT_EQ((smaller2 <=> listWithValues), std::strong_ordering::less);
}

TEST_F(ListTest, SingleElementList) {
    List<int> single{42};

    EXPECT_EQ(single.size(), 1);
    EXPECT_EQ(single.front(), 42);
    EXPECT_EQ(single.back(), 42);

    single.pop_front();
    EXPECT_TRUE(single.empty());

    single.push_back(100);
    EXPECT_EQ(single.size(), 1);
    EXPECT_EQ(single.front(), 100);
    EXPECT_EQ(single.back(), 100);

    single.pop_back();
    EXPECT_TRUE(single.empty());
}

TEST_F(ListTest, LargeList) {
    const int largeSize = 10000;
    List<int> largeList;

    for (int i = 0; i < largeSize; ++i) {
        largeList.push_back(i);
    }

    EXPECT_EQ(largeList.size(), largeSize);
    EXPECT_EQ(largeList.front(), 0);
    EXPECT_EQ(largeList.back(), largeSize - 1);

    int count = 0;
    for (auto it = largeList.begin(); it != largeList.end(); ++it) {
        EXPECT_EQ(*it, count++);
    }

    largeList.clear();
    EXPECT_TRUE(largeList.empty());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}