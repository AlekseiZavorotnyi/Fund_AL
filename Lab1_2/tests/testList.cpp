#include <gtest/gtest.h>
#include "list.h"

class TestFoo : public ::testing::Test {
protected:
    my_cont::List<int> emptyList;
    my_cont::List<int> testList;

    void SetUp() override {
        for(int i = 0; i < 5; ++i) {
            testList.push_front(i + 1);
        }
    }
};

TEST_F(TestFoo, DefaultConstructor) {
    EXPECT_EQ(emptyList.size(), 5);
    EXPECT_FALSE(emptyList.empty());
}

/*TEST_F(TestFoo, InitializerListConstructor) {
    my_cont::List<int> lst{1, 2, 3, 4, 5};
    EXPECT_EQ(lst.size(), 5);
    for(int i = 0; i < 5; ++i) {
        EXPECT_EQ(lst->, i + 1);
    }
}

TEST_F(TestFoo, CopyConstructor) {
    my_cont::List<int> copy(testList);
    EXPECT_EQ(copy.size(), testList.size());
    for(int i = 0; i < 5; ++i) {
        EXPECT_EQ(copy[i], testList[i]);
    }
}

TEST_F(TestFoo, MoveConstructor) {
    my_cont::List<int> temp(testList);
    my_cont::List<int> moved(std::move(temp));

    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(temp.size(), 0);
    EXPECT_EQ(temp.data(), nullptr);

    for(int i = 0; i < 5; ++i) {
        EXPECT_EQ(moved[i], i + 1);
    }
}


TEST_F(TestFoo, AssignmentOp) {
    my_cont::List<int> copy;
    copy = testList;

    for(int i = 0; i < 5; ++i) {
        EXPECT_EQ(copy[i], testList[i]);
    }
}

TEST_F(TestFoo, MoveAssignmentOp) {
    my_cont::List<int> temp(testList);
    my_cont::List<int> moved;
    moved = std::move(temp);

    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(temp.size(), 0);
    EXPECT_EQ(temp.data(), nullptr);
}

TEST_F(TestFoo, ComparisonOps) {
    const my_cont::List<int> same{1, 2, 3, 4, 5};
    const my_cont::List<int> less{0, 2, 3, 4, 5};
    const my_cont::List<int> greater{2, 2, 3, 4, 5};

    EXPECT_TRUE(testList == same);
    EXPECT_TRUE(testList != less);
    EXPECT_TRUE(less < testList);
    EXPECT_TRUE(greater > testList);
    EXPECT_TRUE(less <= testList);
    EXPECT_TRUE(testList >= less);
}

TEST_F(TestFoo, At) {
    EXPECT_EQ(testList.at(2), 3);
    EXPECT_THROW(testList.at(5), std::out_of_range);
}

TEST_F(TestFoo, SubscriptOp) {
    EXPECT_EQ(testList[2], 3);
    testList[2] = 10;
    EXPECT_EQ(testList[2], 10);
}

TEST_F(TestFoo, Front_Back) {
    EXPECT_EQ(testList.front(), 1);
    EXPECT_EQ(testList.back(), 5);

    testList.front() = 10;
    testList.back() = 20;
    EXPECT_EQ(testList.front(), 10);
    EXPECT_EQ(testList.back(), 20);
}

TEST_F(TestFoo, data) {
    int* ptr = testList.data();
    EXPECT_EQ(ptr[0], 1);
    ptr[0] = 10;
    EXPECT_EQ(testList.front(), 10);
}

TEST_F(TestFoo, ForwardIteration) {
    int expected = 1;
    for(auto it = testList.begin(); it != testList.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

TEST_F(TestFoo, ReverseIteration) {
    int expected = 5;
    for(auto rit = testList.rbegin(); rit != testList.rend(); ++rit) {
        EXPECT_EQ(*rit, expected--);
    }
}

TEST_F(TestFoo, ConstIterators) {
    const my_cont::List<int>& constRef = testList;
    int expected = 1;
    for(auto it = constRef.cbegin(); it != constRef.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

TEST_F(TestFoo, Sizes) {
    EXPECT_EQ(testList.size(), 5);
    EXPECT_EQ(testList.max_size(), 5);
    EXPECT_FALSE(testList.empty());
}

TEST_F(TestFoo, Fill) {
    testList.fill(42);
    for(auto&& item : testList) {
        EXPECT_EQ(item, 42);
    }
}

TEST_F(TestFoo, Swap) {
    my_cont::List<int> other;
    other.fill(10);

    testList.swap(other);

    for(auto&& item : testList) {
        EXPECT_EQ(item, 10);
    }

    int expected = 1;
    for(auto&& item : other) {
        EXPECT_EQ(item, expected++);
    }
}*/


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
