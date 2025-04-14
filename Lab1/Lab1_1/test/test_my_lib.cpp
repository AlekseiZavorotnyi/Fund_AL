#include <gtest/gtest.h>
#include "my_lib.h"

class TestFoo : public ::testing::Test {
};

TEST_F(TestFoo, test_add)
{
    Array<int, 10> a = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    ASSERT_EQ(a.size(), 10);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
