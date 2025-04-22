#include <gtest/gtest.h>
#include "my_lib.h"

class TestFoo : public ::testing::Test {
protected:
    my_cont::Array<int, 5> emptyArray;
    my_cont::Array<int, 5> testArray;

    void SetUp() override {
        for(int i = 0; i < 5; ++i) {
            testArray[i] = i + 1;
        }
    }
    void TearDown() override{}
};

TEST_F(TestFoo, DefaultConstructor) {
    EXPECT_EQ(emptyArray.size(), 5);
    EXPECT_FALSE(emptyArray.empty());
}

TEST_F(TestFoo, InitializerListConstructor) {
    my_cont::Array<int, 5> arr{1, 2, 3, 4, 5};
    EXPECT_EQ(arr.size(), 5);
    for(int i = 0; i < 5; ++i) {
        EXPECT_EQ(arr[i], i + 1);
    }
}

TEST_F(TestFoo, CopyConstructor) {
    my_cont::Array<int, 5> copy(testArray);
    EXPECT_EQ(copy.size(), testArray.size());
    for(int i = 0; i < 5; ++i) {
        EXPECT_EQ(copy[i], testArray[i]);
    }
}

TEST_F(TestFoo, MoveConstructor) {
    my_cont::Array<int, 5> temp(testArray);
    my_cont::Array<int, 5> moved(std::move(temp));

    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(temp.size(), 0);
    EXPECT_EQ(temp.data(), nullptr);

    for(int i = 0; i < 5; ++i) {
        EXPECT_EQ(moved[i], i + 1);
    }
}


TEST_F(TestFoo, AssignmentOp) {
    my_cont::Array<int, 5> copy;
    copy = testArray;

    for(int i = 0; i < 5; ++i) {
        EXPECT_EQ(copy[i], testArray[i]);
    }
}

TEST_F(TestFoo, MoveAssignmentOp) {
    my_cont::Array<int, 5> temp(testArray);
    my_cont::Array<int, 5> moved;
    moved = std::move(temp);

    EXPECT_EQ(moved.size(), 5);
    EXPECT_EQ(temp.size(), 0);
    EXPECT_EQ(temp.data(), nullptr);
}

TEST_F(TestFoo, ComparisonOps) {
    const my_cont::Array<int, 5> same{1, 2, 3, 4, 5};
    const my_cont::Array<int, 5> less{0, 2, 3, 4, 5};
    const my_cont::Array<int, 5> greater{2, 2, 3, 4, 5};

    EXPECT_TRUE(testArray == same);
    EXPECT_TRUE(testArray != less);
    EXPECT_TRUE(less < testArray);
    EXPECT_TRUE(greater > testArray);
    EXPECT_TRUE(less <= testArray);
    EXPECT_TRUE(testArray >= less);
}

TEST_F(TestFoo, At) {
    EXPECT_EQ(testArray.at(2), 3);
    EXPECT_THROW(testArray.at(5), std::out_of_range);
}

TEST_F(TestFoo, SubscriptOp) {
    EXPECT_EQ(testArray[2], 3);
    testArray[2] = 10;
    EXPECT_EQ(testArray[2], 10);
}

TEST_F(TestFoo, Front_Back) {
    EXPECT_EQ(testArray.front(), 1);
    EXPECT_EQ(testArray.back(), 5);

    testArray.front() = 10;
    testArray.back() = 20;
    EXPECT_EQ(testArray.front(), 10);
    EXPECT_EQ(testArray.back(), 20);
}

TEST_F(TestFoo, data) {
    int* ptr = testArray.data();
    EXPECT_EQ(ptr[0], 1);
    ptr[0] = 10;
    EXPECT_EQ(testArray.front(), 10);
}

TEST_F(TestFoo, ForwardIteration) {
    int expected = 1;
    for(auto it = testArray.begin(); it != testArray.end(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

TEST_F(TestFoo, ReverseIteration) {
    int expected = 5;
    for(auto rit = testArray.rbegin(); rit != testArray.rend(); ++rit) {
        EXPECT_EQ(*rit, expected--);
    }
}

TEST_F(TestFoo, ConstIterators) {
    const my_cont::Array<int, 5>& constRef = testArray;
    int expected = 1;
    for(auto it = constRef.cbegin(); it != constRef.cend(); ++it) {
        EXPECT_EQ(*it, expected++);
    }
}

TEST_F(TestFoo, Sizes) {
    EXPECT_EQ(testArray.size(), 5);
    EXPECT_EQ(testArray.max_size(), 5);
    EXPECT_FALSE(testArray.empty());
}

TEST_F(TestFoo, Fill) {
    testArray.fill(42);
    for(auto&& item : testArray) {
        EXPECT_EQ(item, 42);
    }
}

TEST_F(TestFoo, Swap) {
    my_cont::Array<int, 5> other;
    other.fill(10);

    testArray.swap(other);

    for(auto&& item : testArray) {
        EXPECT_EQ(item, 10);
    }

    int expected = 1;
    for(auto&& item : other) {
        EXPECT_EQ(item, expected++);
    }
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    int result = RUN_ALL_TESTS();
    // std::cout<< "RESULT : " << result << std::endl;
    // if (result != 0) {
    //     std::cerr << "Some tests failed." << std::endl;
    // }
    return result;
}
