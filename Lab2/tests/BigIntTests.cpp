#include <gtest/gtest.h>
#include "big_int.h"
#include <sstream>

// Тесты для конструкторов
TEST(ConstructorsTest, DefaultConstructor) {
    BigInt num;
    EXPECT_EQ(num, BigInt(0));
}

TEST(ConstructorsTest, IntConstructor) {
    BigInt num1(12345);
    EXPECT_EQ(num1, BigInt("12345"));

    BigInt num2(-6789);
    EXPECT_EQ(num2, BigInt("-6789"));

    BigInt num3(0);
    EXPECT_EQ(num3, BigInt("0"));
}

TEST(ConstructorsTest, StringConstructor) {
    BigInt num1("12345678901234567890");
    EXPECT_EQ(num1, BigInt("12345678901234567890"));

    BigInt num2("-98765432109876543210");
    EXPECT_EQ(num2, BigInt("-98765432109876543210"));

    EXPECT_THROW(BigInt("abc123"), std::invalid_argument);
    EXPECT_THROW(BigInt("12a34"), std::invalid_argument);
}

TEST(ConstructorsTest, CopyMoveConstructor) {
    BigInt num1("12345678901234567890");
    BigInt num2(num1);
    EXPECT_EQ(num1, num2);

    BigInt num3("98765432109876543210");
    BigInt num4(std::move(num3));
    EXPECT_EQ(num4, BigInt("98765432109876543210"));
}

// Тесты для операторов присваивания
TEST(AssignmentTest, CopyAssignment) {
    BigInt num1("12345678901234567890");
    BigInt num2;
    num2 = num1;
    EXPECT_EQ(num1, num2);
}

TEST(AssignmentTest, MoveAssignment) {
    BigInt num1("12345678901234567890");
    BigInt num2;
    num2 = std::move(num1);
    EXPECT_EQ(num2, BigInt("12345678901234567890"));
}

// Тесты для операторов сравнения
TEST(ComparisonTest, Equality) {
    BigInt num1("12345678901234567890");
    BigInt num2("12345678901234567890");
    BigInt num3("98765432109876543210");

    EXPECT_TRUE(num1 == num2);
    EXPECT_FALSE(num1 == num3);
    EXPECT_TRUE(num1 != num3);
    EXPECT_FALSE(num1 != num2);
}

TEST(ComparisonTest, Relational) {
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    BigInt num3("-12345678901234567890");

    EXPECT_TRUE(num1 < num2);
    EXPECT_TRUE(num3 < num1);
    EXPECT_TRUE(num2 > num1);
    EXPECT_TRUE(num1 > num3);
    EXPECT_TRUE(num1 <= num2);
    EXPECT_TRUE(num1 <= num1);
    EXPECT_TRUE(num2 >= num1);
    EXPECT_TRUE(num1 >= num1);
}

// Тесты для арифметических операций
TEST(ArithmeticTest, Addition) {
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    BigInt num3("-12345678901234567890");

    EXPECT_EQ(num1 + num2, BigInt("111111111011111111100"));
    EXPECT_EQ(num1 + num3, BigInt(0));
    EXPECT_EQ(num3 + num1, BigInt(0));
    EXPECT_EQ(num2 + num3, BigInt("86419753208641975320"));
}

TEST(ArithmeticTest, Subtraction) {
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    BigInt num3("-12345678901234567890");

    EXPECT_EQ(num2 - num1, BigInt("86419753208641975320"));
    EXPECT_EQ(num1 - num2, BigInt("-86419753208641975320"));
    EXPECT_EQ(num1 - num3, BigInt("24691357802469135780"));
    EXPECT_EQ(num3 - num1, BigInt("-24691357802469135780"));
}

TEST(ArithmeticTest, Multiplication) {
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    BigInt num3("-12345678901234567890");
    BigInt zero(0);

    EXPECT_EQ(num1 * num2, BigInt("1219326311370217952237463801111263526900"));
    EXPECT_EQ(num1 * num3, BigInt("-152415787532388367501905199875019052100"));
    EXPECT_EQ(num3 * num3, BigInt("152415787532388367501905199875019052100"));
    EXPECT_EQ(num1 * zero, zero);
}

TEST(ArithmeticTest, Division) {
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    BigInt num3("-12345678901234567890");
    BigInt zero(0);

    EXPECT_EQ(num2 / num1, BigInt(8));
    EXPECT_EQ(num1 / num2, BigInt(0));
    EXPECT_EQ(num3 / num1, BigInt(-1));
    EXPECT_EQ((num2 + num1) / num1, BigInt(9));
    EXPECT_THROW(num1 / zero, std::invalid_argument);
}

TEST(ArithmeticTest, Modulo) {
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");

    EXPECT_EQ(num2 % num1, BigInt("900000000090"));
    EXPECT_EQ(num1 % BigInt("10000000000000000000"), BigInt("2345678901234567890"));
}

// Тесты для составных операторов присваивания
TEST(CompoundAssignmentTest, PlusEquals) {
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    num1 += num2;
    EXPECT_EQ(num1, BigInt("111111111011111111100"));
}

TEST(CompoundAssignmentTest, MinusEquals) {
    BigInt num1("98765432109876543210");
    BigInt num2("12345678901234567890");
    num1 -= num2;
    EXPECT_EQ(num1, BigInt("86419753208641975320"));
}

TEST(CompoundAssignmentTest, MultiplyEquals) {
    BigInt num1("12345678901234567890");
    BigInt num2("10000000000000000000");
    num1 *= num2;
    EXPECT_EQ(num1, BigInt("123456789012345678900000000000000000000"));
}

TEST(CompoundAssignmentTest, DivideEquals) {
    BigInt num1("123456789012345678900000000000000000000");
    BigInt num2("10000000000000000000");
    num1 /= num2;
    EXPECT_EQ(num1, BigInt("12345678901234567890"));
}

// Тесты для инкремента/декремента
TEST(IncrementDecrementTest, PrefixIncrement) {
    BigInt num1("99999999999999999999");
    ++num1;
    EXPECT_EQ(num1, BigInt("100000000000000000000"));

    BigInt num2("-1");
    ++num2;
    EXPECT_EQ(num2, BigInt(0));
}

TEST(IncrementDecrementTest, PrefixDecrement) {
    BigInt num1("100000000000000000000");
    --num1;
    EXPECT_EQ(num1, BigInt("99999999999999999999"));

    BigInt num2("1");
    --num2;
    EXPECT_EQ(num2, BigInt(0));
}

// Тесты для специальных методов
TEST(MethodsTest, Abs) {
    BigInt num1("12345678901234567890");
    BigInt num2("-12345678901234567890");

    EXPECT_EQ(num1.abs(), BigInt("12345678901234567890"));
    EXPECT_EQ(num2.abs(), BigInt("12345678901234567890"));
}

// Тесты для ввода/вывода
TEST(IOStreamTest, OutputOperator) {
    BigInt num("12345678901234567890");
    std::ostringstream oss;
    oss << num;
    EXPECT_EQ(oss.str(), "12345678901234567890");

    BigInt neg_num("-98765432109876543210");
    std::ostringstream oss_neg;
    oss_neg << neg_num;
    EXPECT_EQ(oss_neg.str(), "-98765432109876543210");
}

TEST(IOStreamTest, InputOperator) {
    BigInt num;
    std::istringstream iss("12345678901234567890");
    iss >> num;
    EXPECT_EQ(num, BigInt("12345678901234567890"));

    BigInt neg_num;
    std::istringstream iss_neg("-98765432109876543210");
    iss_neg >> neg_num;
    EXPECT_EQ(neg_num, BigInt("-98765432109876543210"));
}

// Тесты для модульного возведения в степень
TEST(ModularExponentiationTest, ModExp) {
    BigInt base("12345678901234567890");
    BigInt exp("20");
    BigInt mod("10000000000000000000");

    BigInt result = base.mod_exp(exp, mod);
    EXPECT_EQ(result, BigInt("0"));
}

// Тесты для алгоритма Карацубы
TEST(KaratsubaTest, Multiplication) {
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");

    BigInt karatsuba_result = num1.karatsuba_multiply(num2);
    BigInt normal_result = num1 * num2;
    EXPECT_EQ(karatsuba_result, normal_result);
}

// Тесты для крайних случаев
TEST(EdgeCasesTest, LargeNumbers) {
    BigInt zero(0);
    BigInt very_large("99999999999999999999999999999999999999");
    BigInt very_large_plus_one("100000000000000000000000000000000000000");

    EXPECT_EQ(very_large + BigInt(1), very_large_plus_one);
    EXPECT_EQ(very_large_plus_one - BigInt(1), very_large);
    EXPECT_EQ(very_large * zero, zero);
    EXPECT_THROW(very_large / zero, std::invalid_argument);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}