#include <gtest/gtest.h>
#include "unique_ptr.h"

using namespace my_cont;

class VectorTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Инициализация перед каждым тестом
        int_vec = Vector<int>{1, 2, 3};
        empty_vec = Vector<int>{};
    }

    // Объявление переменных для тестов
    Vector<int> int_vec;
    Vector<int> empty_vec;
};

TEST_F(VectorTest, DefaultConstructor) {
    Vector<int> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(v.size(), 0);
    EXPECT_GE(v.capacity(), 0);
}

TEST_F(VectorTest, InitializerListConstructor) {
    Vector<int> v{1, 2, 3};
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 1);
    EXPECT_EQ(v[1], 2);
    EXPECT_EQ(v[2], 3);
}

TEST_F(VectorTest, CopyConstructor) {
    Vector<int> copy(int_vec);
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
}

TEST_F(VectorTest, MoveConstructor) {
    Vector<int> moved(std::move(int_vec));
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
    EXPECT_TRUE(int_vec.empty());
}

TEST_F(VectorTest, CopyAssignment) {
    Vector<int> copy;
    copy = int_vec;
    EXPECT_EQ(copy.size(), 3);
    EXPECT_EQ(copy[0], 1);
    EXPECT_EQ(copy[1], 2);
    EXPECT_EQ(copy[2], 3);
}

TEST_F(VectorTest, MoveAssignment) {
    Vector<int> moved;
    moved = std::move(int_vec);
    EXPECT_EQ(moved.size(), 3);
    EXPECT_EQ(moved[0], 1);
    EXPECT_EQ(moved[1], 2);
    EXPECT_EQ(moved[2], 3);
    EXPECT_TRUE(int_vec.empty());
}

TEST_F(VectorTest, ElementAccess) {
    EXPECT_EQ(int_vec[0], 1);
    EXPECT_EQ(int_vec.at(1), 2);
    EXPECT_EQ(int_vec.front(), 1);
    EXPECT_EQ(int_vec.back(), 3);
    EXPECT_EQ(int_vec.data()[0], 1);
}

TEST_F(VectorTest, AtThrowsWhenOutOfRange) {
    EXPECT_THROW(empty_vec.at(0), std::out_of_range);
    EXPECT_THROW(int_vec.at(3), std::out_of_range);
}

TEST_F(VectorTest, FrontBackThrowWhenEmpty) {
    EXPECT_THROW(empty_vec.front(), std::out_of_range);
    EXPECT_THROW(empty_vec.back(), std::out_of_range);
}

TEST_F(VectorTest, CapacityMethods) {
    EXPECT_FALSE(int_vec.empty());
    EXPECT_EQ(int_vec.size(), 3);
    EXPECT_GE(int_vec.capacity(), 3);
}

TEST_F(VectorTest, ReserveAndShrink) {
    Vector<int> v;
    v.reserve(100);
    EXPECT_GE(v.capacity(), 100);
    v.push_back(1);
    v.push_back(2);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 2);
}

TEST_F(VectorTest, PushBack) {
    Vector<int> v;
    v.push_back(1);
    EXPECT_EQ(v.size(), 1);
    EXPECT_EQ(v[0], 1);
    v.push_back(2);
    EXPECT_EQ(v.size(), 2);
    EXPECT_EQ(v[1], 2);
}

TEST_F(VectorTest, PopBack) {
    int_vec.pop_back();
    EXPECT_EQ(int_vec.size(), 2);
    EXPECT_EQ(int_vec.back(), 2);
}

TEST_F(VectorTest, PopBackThrowsWhenEmpty) {
    EXPECT_THROW(empty_vec.pop_back(), std::out_of_range);
}

TEST_F(VectorTest, Insert) {
    int_vec.insert(1, 10);
    EXPECT_EQ(int_vec.size(), 4);
    EXPECT_EQ(int_vec[0], 1);
    EXPECT_EQ(int_vec[1], 10);
    EXPECT_EQ(int_vec[2], 2);
    EXPECT_EQ(int_vec[3], 3);
}

TEST_F(VectorTest, Erase) {
    int_vec.erase(1);
    EXPECT_EQ(int_vec.size(), 2);
    EXPECT_EQ(int_vec[0], 1);
    EXPECT_EQ(int_vec[1], 3);
}

TEST_F(VectorTest, Resize) {
    int_vec.resize(5, 10);
    EXPECT_EQ(int_vec.size(), 5);
    EXPECT_EQ(int_vec[3], 10);
    EXPECT_EQ(int_vec[4], 10);

    int_vec.resize(2);
    EXPECT_EQ(int_vec.size(), 2);
}

TEST_F(VectorTest, Clear) {
    int_vec.clear();
    EXPECT_TRUE(int_vec.empty());
    EXPECT_EQ(int_vec.size(), 0);
}

TEST_F(VectorTest, Swap) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{4, 5};
    v1.swap(v2);
    EXPECT_EQ(v1.size(), 2);
    EXPECT_EQ(v2.size(), 3);
    EXPECT_EQ(v1[0], 4);
    EXPECT_EQ(v2[0], 1);
}

TEST_F(VectorTest, ComparisonOperators) {
    Vector<int> v1{1, 2, 3};
    Vector<int> v2{1, 2, 3};
    Vector<int> v3{1, 2};
    Vector<int> v4{1, 2, 4};

    EXPECT_TRUE(v1 == v2);
    EXPECT_TRUE(v1 != v3);
    EXPECT_TRUE(v3 < v1);
    EXPECT_TRUE(v1 <= v2);
    EXPECT_TRUE(v4 > v1);
    EXPECT_TRUE(v1 >= v3);

    EXPECT_EQ(v1 <=> v2, std::strong_ordering::equal);
    EXPECT_EQ(v1 <=> v3, std::strong_ordering::greater);
    EXPECT_EQ(v3 <=> v1, std::strong_ordering::less);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}