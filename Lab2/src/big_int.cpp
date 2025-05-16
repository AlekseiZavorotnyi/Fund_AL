#include "big_int.h"
#include <algorithm>
#include <complex>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <string>

using ll = long long;
using ull = unsigned long long;

std::size_t num_length(uint64_t num) {
    std::size_t length = 0;
    while (num) {
        num /= 10;
        length++;
    }
    return length;
}

BigInt::BigInt() {
    isNegative = false;
    digits.push_back(0);
}

BigInt::BigInt(long long value) {
    isNegative = false;
    if (value < 0) {
        isNegative = true;
        value = -value;
    }
    if (value == 0) {
        digits.push_back(0);
    } else {
        while (value != 0) {
            digits.push_back(value % BASE);
            value /= BASE;
        }
    }
}

BigInt::BigInt(const std::string &str) {
    std::string temp = str;
    if (temp.empty()) {
        digits.push_back(0);
        isNegative = false;
        return;
    }
    isNegative = false;
    if (temp[0] == '-') {
        isNegative = true;
        temp = temp.substr(1);
    }
    if (!std::all_of(temp.begin(), temp.end(), ::isdigit)) {
        throw std::invalid_argument("invalid number");
    }

    for (long long i = temp.length(); i > 0; i -= 9) {
        if (i < 9) {
            digits.push_back(atoll(temp.substr(0, i).c_str()));
        } else {
            digits.push_back(atoll(temp.substr(i - 9, 9).c_str()));
        }
    }
    remove_leading_zeros();
}

BigInt::BigInt(const BigInt& other) {
    isNegative = other.isNegative;
    digits = other.digits;
}

BigInt::BigInt(BigInt&& other) noexcept {
    isNegative = other.isNegative;
    digits = std::move(other.digits);
}

BigInt::~BigInt() {
    digits.clear();
}

std::ostream &operator<<(std::ostream &os, const BigInt &num) {
    if (num.digits.empty() || (num.digits.size() == 1 && num.digits[0] == 0)) {
        os << "0";
    } else {
        if (num.isNegative) {
            os << "-";
        }
        for (ll i = num.digits.size() - 1; i >= 0; i--) {
            if (num.digits[i] == 0) {
                os << "000000000";
            } else {
                if (i != static_cast<long long>(num.digits.size()) - 1) {
                    std::size_t len = num_length(num.digits[i]);
                    if (len < 9) {
                        std::string s(9 - len, '0');
                        os << s;
                    }
                }
                os << num.digits[i];
            }
        }
    }
    return os;
}

std::istream& operator>>(std::istream& is, BigInt& num) {
    std::string input;
    is >> input;
    num = BigInt(input);
    return is;
}

BigInt& BigInt::operator=(const BigInt& other) {
    if (this != &other) {
        isNegative = other.isNegative;
        digits = other.digits;
    }
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    if (this != &other) {
        isNegative = other.isNegative;
        digits = std::move(other.digits);
    }
    return *this;
}

bool BigInt::operator==(const BigInt &other) const {
    if (isNegative != other.isNegative) return false;
    if (digits.size() != other.digits.size()) return false;
    for (size_t i = 0; i < digits.size(); ++i) {
        if (digits[i] != other.digits[i]) return false;
    }
    return true;
}

bool BigInt::operator!=(const BigInt &other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt &other) const {
    if (isNegative != other.isNegative) {
        return isNegative;
    }
    if (digits.size() != other.digits.size()) {
        return (digits.size() < other.digits.size()) ^ isNegative;
    }
    for (int i = digits.size() - 1; i >= 0; --i) {
        if (digits[i] != other.digits[i]) {
            return (digits[i] < other.digits[i]) ^ isNegative;
        }
    }
    return false;
}

bool BigInt::operator>(const BigInt &other) const {
    return other < *this;
}

bool BigInt::operator<=(const BigInt &other) const {
    return !(other < *this);
}

bool BigInt::operator>=(const BigInt &other) const {
    return !(*this < other);
}

void BigInt::remove_leading_zeros() {
    while (digits.size() > 1 && digits.back() == 0) {
        digits.pop_back();
    }
    if (digits.empty()) {
        digits.push_back(0);
    }
}

BigInt BigInt::abs() const {
    BigInt temp = BigInt(*this);
    temp.isNegative = false;
    return temp;
}

BigInt BigInt::operator+(const BigInt& other) const {
    if (isNegative == other.isNegative) {
        BigInt result;
        result.isNegative = isNegative;

        ull carry = 0;
        size_t max_size = std::max(digits.size(), other.digits.size());
        result.digits.resize(max_size, 0);

        for (size_t i = 0; i < max_size || carry; ++i) {
            if (i == result.digits.size()) {
                result.digits.push_back(0);
            }
            ull sum = carry;
            if (i < digits.size()) sum += digits[i];
            if (i < other.digits.size()) sum += other.digits[i];
            carry = sum / BASE;
            result.digits[i] = sum % BASE;
        }

        result.remove_leading_zeros();
        return result;
    } else {
        if (isNegative) {
            return other - this->abs();
        } else {
            return *this - other.abs();
        }
    }
}

BigInt BigInt::operator-(const BigInt& other) const {
    if (isNegative != other.isNegative) {
        BigInt tmp = other;
        tmp.isNegative = !other.isNegative;
        return *this + tmp;
    }

    if (abs() < other.abs()) {
        BigInt result = other - *this;
        result.isNegative = !isNegative;
        return result;
    }

    BigInt result;
    result.isNegative = isNegative;
    result.digits.resize(digits.size(), 0);

    ull borrow = 0;
    for (size_t i = 0; i < digits.size(); ++i) {
        ull diff = digits[i] - borrow;
        if (i < other.digits.size()) {
            diff -= other.digits[i];
        }
        borrow = (diff >= BASE) ? 1 : 0;
        if (borrow) diff += BASE;
        result.digits[i] = diff % BASE;
    }

    result.remove_leading_zeros();
    return result;
}

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt result;
    result.isNegative = isNegative != other.isNegative;
    result.digits.resize(digits.size() + other.digits.size(), 0);

    for (size_t i = 0; i < digits.size(); ++i) {
        ull carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            ull product = result.digits[i + j] + carry;
            if (j < other.digits.size()) {
                product += digits[i] * other.digits[j];
            }
            carry = product / BASE;
            result.digits[i + j] = product % BASE;
        }
    }

    result.remove_leading_zeros();
    if (result.digits.size() == 1 && result.digits[0] == 0) {
        result.isNegative = false;
    }
    return result;
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (other == BigInt(0)) {
        throw std::invalid_argument("Division by zero");
    }

    BigInt abs_other = other.abs();
    if (abs() < abs_other) {
        return BigInt(0);
    }

    BigInt result, current;
    result.isNegative = isNegative != other.isNegative;
    result.digits.resize(digits.size(), 0);

    for (int i = digits.size() - 1; i >= 0; --i) {
        current.digits.insert(current.digits.begin(), digits[i]);
        current.remove_leading_zeros();

        ull l = 0, r = BASE;
        while (l < r) {
            ull m = (l + r + 1) / 2;
            BigInt product = abs_other * BigInt(m);
            if (product <= current) {
                l = m;
            } else {
                r = m - 1;
            }
        }

        result.digits[i] = l;
        current -= abs_other * BigInt(l);
    }

    result.remove_leading_zeros();
    if (result.digits.size() == 1 && result.digits[0] == 0) {
        result.isNegative = false;
    }
    return result;
}

BigInt BigInt::operator+=(const BigInt& other) {
    *this = *this + other;
    return *this;
}

BigInt BigInt::operator-=(const BigInt& other) {
    *this = *this - other;
    return *this;
}

BigInt BigInt::operator*=(const BigInt& other) {
    *this = *this * other;
    return *this;
}

BigInt BigInt::operator/=(const BigInt& other) {
    *this = *this / other;
    return *this;
}

BigInt BigInt::operator++() {
    *this += BigInt(1);
    return *this;
}

BigInt BigInt::operator--() {
    *this -= BigInt(1);
    return *this;
}

BigInt BigInt::operator%(const BigInt& other) const {
    if (*this >= other) {
        BigInt res = *this - (*this / other) * other;
        return res;
    }
    return *this;
}

BigInt BigInt::mod_exp(const BigInt& exp, const BigInt& mod) const {
    if (exp < BigInt(2)) {
        if (exp == BigInt(1)) {
            return *this % mod;
        }
        return BigInt(1);
    }
    BigInt res = mod_exp(exp / BigInt(2), mod);
    res = (res * res) % mod;
    if (exp.digits[0] % 2 == 1) {
        res = (res * *this) % mod;
    }
    return res;
}

void BigInt::split_at(const BigInt& num, size_t m, BigInt& high, BigInt& low) {
    if (m >= num.digits.size()) {
        high = BigInt(0);
        low = num;
    } else {
        high.digits.assign(num.digits.begin() + m, num.digits.end());
        low.digits.assign(num.digits.begin(), num.digits.begin() + m);
        high.remove_leading_zeros();
        low.remove_leading_zeros();
    }
    high.isNegative = num.isNegative;
    low.isNegative = num.isNegative;
}

BigInt BigInt::shift_left(size_t m) const {
    if (*this == BigInt(0)) {
        return *this;
    }

    BigInt result(*this);
    result.digits.insert(result.digits.begin(), m, 0);
    return result;
}

BigInt BigInt::karatsuba_multiply(const BigInt& other) const {
    if (digits.size() <= 10 || other.digits.size() <= 10) {
        return *this * other;
    }

    size_t m = std::max(digits.size(), other.digits.size());
    m = m / 2 + m % 2;

    BigInt a, b, c, d;
    split_at(*this, m, a, b);
    split_at(other, m, c, d);

    BigInt ac = a.karatsuba_multiply(c);
    BigInt bd = b.karatsuba_multiply(d);
    BigInt ab_cd = (a + b).karatsuba_multiply(c + d);

    BigInt ad_bc = ab_cd - ac - bd;

    BigInt result = ac.shift_left(2 * m) + ad_bc.shift_left(m) + bd;

    result.isNegative = (this->isNegative != other.isNegative);
    if (result.digits.size() == 1 && result.digits[0] == 0) {
        result.isNegative = false;
    }

    return result;
}

void BigInt::fft(std::vector<std::complex<long double>>& a, bool invert)
{
    auto size = a.size();
    if (size == 1)  return;

    std::vector<std::complex<long double>> a0(size / 2);
    std::vector<std::complex<long double>> a1(size / 2);
    for (unsigned i = 0, j = 0; i < size; i += 2, j++)
    {
        a0[j] = a[i];
        a1[j] = a[i + 1];
    }

    fft(a0, invert);
    fft(a1, invert);

    long double ang = 2 * ((long double)(M_PI)) / size * (invert ? -1 : 1);
    std::complex<long double> w(1.0);
    std::complex<long double> wn(cosl(ang), sinl(ang));
    for (unsigned int i = 0; i < size / 2; ++i) {
        a[i] = a0[i] + w * a1[i];
        a[i + size / 2] = a0[i] - w * a1[i];
        if (invert)
            a[i] /= 2, a[i + size / 2] /= 2;
        w *= wn;
    }
}



BigInt BigInt::multFurie(const BigInt& second) const {
    BigInt result;
    result.isNegative = isNegative != second.isNegative;

    // Подготовка векторов для FFT
    std::vector<std::complex<long double>> fa(digits.begin(), digits.end());
    std::vector<std::complex<long double>> fb(second.digits.begin(), second.digits.end());

    // Находим ближайшую степень двойки, достаточную для хранения результата
    size_t n = 1;
    while (n < std::max(fa.size(), fb.size())) n <<= 1;
    n <<= 1;
    fa.resize(n);
    fb.resize(n);

    // Выполняем прямое преобразование Фурье
    fft(fa, false);
    fft(fb, false);

    // Поэлементное умножение
    for (size_t i = 0; i < n; ++i)
        fa[i] *= fb[i];

    // Обратное преобразование Фурье
    fft(fa, true);

    // Обработка результатов и переносы
    unsigned long long carry = 0;
    for (auto &x : fa) {
        unsigned long long current = (unsigned long long)(x.real() + 0.5) + carry;
        carry = current / BASE;
        result.digits.push_back(current % BASE);
    }

    // Добавляем оставшийся перенос
    while (carry) {
        result.digits.push_back(carry % BASE);
        carry /= BASE;
    }

    result.remove_leading_zeros();
    return result;
}