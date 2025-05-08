#include <gtest/gtest.h>
#include "big_int.h"



// Constructor tests
TEST(Constructor, DefaultConstructor) {
    const BigInt num;
    EXPECT_EQ(num, BigInt(0));
}

TEST(Constructor, LongLongConstructor) {
    BigInt num1(123456789);
    BigInt num2(-987654321);
    BigInt num3(0);

    EXPECT_EQ(num1, BigInt("123456789"));
    EXPECT_EQ(num2, BigInt("-987654321"));
    EXPECT_EQ(num3, BigInt("0"));
}

TEST(Constructor, StringConstructor) {
    BigInt num1("123456789012345678");
    BigInt num2("-987654321098765430");
    BigInt num3("0");

    EXPECT_EQ(num1, BigInt(123456789012345678));
    EXPECT_EQ(num2, BigInt(-98765432109876543) * BigInt(10));
    EXPECT_EQ(num3, BigInt(0));

    EXPECT_THROW(BigInt("abc123"), std::invalid_argument);
    EXPECT_THROW(BigInt("12a34"), std::invalid_argument);
}

TEST(Constructor, CopyMoveConstructors) {
    BigInt num1("123456789");
    BigInt num2(num1);
    BigInt num3("987654321");
    BigInt num4(std::move(num3));

    EXPECT_EQ(num1, num2);
    EXPECT_EQ(num4, BigInt("987654321"));
}

// Assignment operators
TEST(Operators, AssignmentOperators) {
    BigInt num1("123456789");
    BigInt num2;
    num2 = num1;
    EXPECT_EQ(num1, num2);

    BigInt num3;
    num3 = std::move(BigInt("987654321"));
    EXPECT_EQ(num3, BigInt("987654321"));
}

// Arithmetic operations
TEST(Operators, Addition) {
    BigInt num1("123456789");
    BigInt num2("987654321");
    BigInt num3("-123456789");
    BigInt num4("-987654321");
    BigInt zero(0);

    EXPECT_EQ(num1 + num2, BigInt("1111111110"));
    EXPECT_EQ(num1 + num3, zero);
    EXPECT_EQ(num3 + num4, BigInt("-1111111110"));
    EXPECT_EQ(num1 + zero, num1);
    EXPECT_EQ(num4 + num1, BigInt("-864197532"));
}

TEST(Operators, Subtraction) {
    BigInt num1("123456789");
    BigInt num2("987654321");
    BigInt num3("-123456789");
    BigInt num4("-987654321");
    BigInt zero(0);

    EXPECT_EQ(num2 - num1, BigInt("864197532"));
    EXPECT_EQ(num1 - num1, zero);
    EXPECT_EQ(num3 - num4, BigInt("864197532"));
    EXPECT_EQ(num4 - num3, BigInt("-864197532"));
    EXPECT_EQ(zero - num1, BigInt("-123456789"));
}

TEST(Operators, Multiplication) {
    BigInt num1("123456789");
    BigInt num2("987654321");
    BigInt num3("-123456789");
    BigInt num4("-987654321");
    BigInt zero(0);

    EXPECT_EQ(num1 * num2, BigInt("121932631112635269"));
    EXPECT_EQ(num1 * num3, BigInt("-15241578750190521"));
    EXPECT_EQ(num3 * num4, BigInt("121932631112635269"));
    EXPECT_EQ(num1 * zero, zero);
    EXPECT_EQ(zero * num4, zero);
}

TEST(Operators, Division) {
    BigInt num1("123456789");
    BigInt num2("987654321");
    BigInt num3("-123456789");
    BigInt num4("-987654321");
    BigInt zero(0);
    BigInt one(1);

    EXPECT_EQ(num2 / num1, BigInt("8"));
    EXPECT_EQ(num1 / num2, zero);
    EXPECT_EQ(num4 / num3, BigInt("8"));
    EXPECT_EQ(num3 / num1, BigInt("-1"));
    EXPECT_EQ(num1 / one, num1);
    EXPECT_THROW(num1 / zero, std::invalid_argument);
}

TEST(Operators, CompoundAssignmentOperators) {
    BigInt num1("12347402428078456789");
    BigInt num2("98765432829729388731");
    BigInt num3("-12347402428078456789");

    num1 += num2;
    EXPECT_EQ(num1, BigInt("111112835257807845520"));

    num1 -= num2;
    EXPECT_EQ(num1, BigInt("12347402428078456789"));

    num1 *= num2;
    EXPECT_EQ(num1, BigInt("1219496545132020383724120990446767044759"));
    num1 /= num2;
    EXPECT_EQ(num1, BigInt("12347402428078456789"));

    num3 += BigInt("12347402428078456789");
    EXPECT_EQ(num3, BigInt(0));
}

// Comparison operators
TEST(Operators, ComparisonOperators) {
    BigInt num1("123456789");
    BigInt num2("987654321");
    BigInt num3("-123456789");
    BigInt num4("-987654321");
    BigInt num5("123456789");

    EXPECT_TRUE(num1 < num2);
    EXPECT_TRUE(num4 < num3);
    EXPECT_TRUE(num1 <= num5);
    EXPECT_TRUE(num1 == num5);
    EXPECT_TRUE(num1 != num2);
    EXPECT_TRUE(num2 > num1);
    EXPECT_TRUE(num3 > num4);
    EXPECT_TRUE(num5 >= num1);
    EXPECT_TRUE(num3 <= num1);
}

// I/O operations
TEST(Operators, StreamOperations) {
    std::stringstream ss1, ss2, ss3;
    BigInt num1("123456789");
    BigInt num2("-987654321");
    BigInt num3;

    ss1 << num1;
    EXPECT_EQ(ss1.str(), "123456789");

    ss2 << num2;
    EXPECT_EQ(ss2.str(), "-987654321");

    ss3 << "123456789";
    ss3 >> num3;
    EXPECT_EQ(num3, num1);

    ss3.clear();
    ss3 << "-987654321";
    ss3 >> num3;
    EXPECT_EQ(num3, num2);

    ss3.clear();
    ss3 << "invalid";
    EXPECT_THROW(ss3 >> num3, std::invalid_argument);
}

TEST(Func, AbsFunction) {
    BigInt num1("123456789");
    BigInt num2("-987654321");

    EXPECT_EQ(num1.abs(), num1);
    EXPECT_EQ(num2.abs(), BigInt("987654321"));
    EXPECT_EQ(BigInt(0).abs(), BigInt(0));
}

TEST(StressTest, EdgeCases) {
    BigInt max_ll(std::to_string(LLONG_MAX));
    BigInt min_ll(std::to_string(LLONG_MIN));
    BigInt beyond_ll("9223372036854775808"); // LL_MAX + 1

    EXPECT_GT(beyond_ll, max_ll);
    EXPECT_LT(min_ll, max_ll);

    BigInt zero(0);
    BigInt num("123456789");

    EXPECT_EQ(num + zero, num);
    EXPECT_EQ(num - zero, num);
    EXPECT_EQ(num * zero, zero);

    BigInt one(1);
    EXPECT_EQ(num * one, num);
    EXPECT_EQ(num / one, num);
}

TEST(StressTest, Performance) {
    BigInt a("123456789012345678901234567890");
    BigInt b("987654321098765432109876543210");

    EXPECT_NO_THROW(a + b);
    EXPECT_NO_THROW(a - b);
    EXPECT_NO_THROW(a * b);
    EXPECT_NO_THROW(a / BigInt("1234567890"));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}