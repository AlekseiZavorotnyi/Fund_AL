#ifndef BIG_INT_H
#define BIG_INT_H

#include <complex>
#include <iostream>
#include <vector>
#include <string>
#include <bits/stdint-uintn.h>

#define BASE 1000000000

class BigInt {
private:
    std::vector<unsigned long long> digits;
    bool isNegative;
    void remove_leading_zeros();
    [[nodiscard]] BigInt shift_left(size_t m) const;
    static void split_at(const BigInt& num, size_t m, BigInt& high, BigInt& low) ;

public:
    BigInt();
    explicit BigInt(long long value);
    explicit BigInt(const std::string& str);
    BigInt(const BigInt& other);
    BigInt(BigInt&& other) noexcept;
    ~BigInt();

    BigInt& operator=(const BigInt& other);
    BigInt& operator=(BigInt&& other) noexcept;

    BigInt operator+(const BigInt& other) const;
    BigInt operator-(const BigInt& other) const;
    BigInt operator*(const BigInt& other) const;
    BigInt operator/(const BigInt& other) const;

    BigInt operator+=(const BigInt& other);
    BigInt operator-=(const BigInt& other);
    BigInt operator*=(const BigInt& other);
    BigInt operator/=(const BigInt& other);
    BigInt operator++();
    BigInt operator--();

    bool operator==(const BigInt& other) const;
    bool operator!=(const BigInt& other) const;
    bool operator<(const BigInt& other) const;
    bool operator>(const BigInt& other) const;
    bool operator<=(const BigInt& other) const;
    bool operator>=(const BigInt& other) const;

    BigInt operator%(const BigInt& other) const;
    [[nodiscard]] BigInt abs() const;

    [[nodiscard]] BigInt mod_exp(const BigInt& exp, const BigInt& mod) const;

    void fft(std::vector<std::complex<long double>>& a, bool invert);

    BigInt multFurie(const BigInt &second);

    [[nodiscard]] BigInt fft_multiply(const BigInt& a) const;

    [[nodiscard]] BigInt karatsuba_multiply(const BigInt& a) const;
    [[nodiscard]] BigInt newton_divide(const BigInt& a) const;


    friend std::istream& operator>>(std::istream& is, BigInt& num);
    friend std::ostream& operator<<(std::ostream& os, const BigInt& num);
};

#endif