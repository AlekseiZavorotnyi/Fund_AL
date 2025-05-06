#include "big_int.h"

#include <algorithm>
#include <cstdint>
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
}

BigInt::BigInt(long long value) {
    isNegative = false;
    if (value < 0) {
        isNegative = true;
        value = -value;
    }
    while (value != 0) {
        digits.push_back(value % BASE);
        value /= BASE;
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

}

BigInt::BigInt(const BigInt& other) {
    isNegative = other.isNegative;
    digits = std::vector<unsigned long long>(other.digits);
}

BigInt::BigInt(BigInt&& other) noexcept {
    isNegative = other.isNegative;
    digits = std::move(other.digits);
}

BigInt::~BigInt() {
    digits.clear();
}

std::ostream &operator<<(std::ostream &os, const BigInt &num) {
    if (num.digits.empty() || num.digits.back() == 0) {
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
    isNegative = other.isNegative;
    digits = std::vector<unsigned long long>(other.digits);
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) noexcept {
    isNegative = other.isNegative;
    digits = std::move(other.digits);
    return *this;
}

bool BigInt::operator==(const BigInt &other) const {
    bool this_zero = (digits.empty()) || (digits.size() == 1 && digits[0] == 0);
    bool other_zero = (other.digits.empty()) || (other.digits.size() == 1 && other.digits[0] == 0);

    if (this_zero && other_zero) {
        return true;
    }

    return (digits == other.digits) && (isNegative == other.isNegative);
}

bool BigInt::operator!=(const BigInt &other) const {
    return !(*this == other);
}

bool BigInt::operator<(const BigInt &other) const {
    if (isNegative && !other.isNegative) {
        return true;
    }
    if (!isNegative && other.isNegative) {
        return false;
    }
    if (digits.size() > other.digits.size()) {
        return false;
    }
    if (digits.size() < other.digits.size()) {
        return true;
    }
    bool findLess = false;
    for (std::size_t i = 0; i < digits.size(); ++i) {
        if (!isNegative) {
            if (digits[i] > other.digits[i]) {
                return false;
            }
            if (digits[i] < other.digits[i]) {
                findLess = true;
            }
        } else {
            if (digits[i] < other.digits[i]) {
                return false;
            }
            if (digits[i] > other.digits[i]) {
                findLess = true;
            }
        }
    }
    return findLess;
}

bool BigInt::operator>(const BigInt &other) const {
    if (*this == other) {
        return false;
    }
    if (*this < other) {
        return false;
    }
    return true;
}

bool BigInt::operator<=(const BigInt &other) const {
    if (*this > other) {
        return false;
    }
    return true;
}

bool BigInt::operator>=(const BigInt &other) const {
    if (*this < other) {
        return false;
    }
    return true;
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
    BigInt res = BigInt();
    res.isNegative = isNegative;
    BigInt abs_cur = this->abs(), abs_other = other.abs();
    if (isNegative && !other.isNegative && abs_cur < abs_other) {
        res.isNegative = false;
    }
    if (!isNegative && other.isNegative && abs_cur < abs_other) {
        res.isNegative = true;
    }
    if (isNegative == other.isNegative) {
        ull sum = 0;
        ull max_size = std::max(digits.size(), other.digits.size());
        ull min_size = std::min(digits.size(), other.digits.size());
        for (size_t i = 0; i < max_size; i++) {
            if (digits.size() > min_size && i >= min_size) {
                sum += digits[i];
            }
            else if(other.digits.size() > min_size && i >= min_size) {
                sum += other.digits[i];
            }
            else {
                sum += digits[i] + other.digits[i];
            }
            res.digits.push_back(sum % BASE);
            sum /= BASE;
        }
        if (sum > 0) {
            res.digits.push_back(sum % BASE);
        }
    }
    else {
        if (abs_cur > abs_other) {
            ull diff = 0;
            ull max_size = digits.size();
            ull min_size = other.digits.size();
            int in_mind = 0;
            for (size_t i = 0; i < min_size; i++) {
                if ((digits[i] - in_mind) >= other.digits[i]) {
                    diff = digits[i] - in_mind - other.digits[i];
                    in_mind = 0;
                }
                else {
                    diff = digits[i] - in_mind + BASE - other.digits[i];
                    in_mind = 1;
                }
                res.digits.push_back(diff);
                diff = 0;
            }
            for (size_t i = min_size; i < max_size; i++) {
                if (!(i == max_size - 1 && (digits[i] - in_mind) == 0)) {
                    res.digits.push_back(digits[i] - in_mind);
                    in_mind = 0;
                }

            }
        }
        else {
            ull diff = 0;
            ull max_size = other.digits.size();
            ull min_size = digits.size();
            int in_mind = 0;
            for (size_t i = 0; i < min_size; i++) {
                if ((other.digits[i] - in_mind) >= digits[i]) {
                    diff = other.digits[i] - in_mind - digits[i];
                    in_mind = 0;
                }
                else {
                    diff = other.digits[i] - in_mind + BASE - digits[i];
                    in_mind = 1;
                }
                res.digits.push_back(diff);
            }
            for (size_t i = min_size; i < max_size; i++) {
                if (!(i == max_size - 1 && (other.digits[i] - in_mind) == 0)) {
                    res.digits.push_back(other.digits[i] - in_mind);
                    in_mind = 0;
                }

            }
        }
    }
    return res;
}

BigInt BigInt::operator-(const BigInt& other) const {
    BigInt res = other;
    res.isNegative = !isNegative;
    return *this + res;
}

BigInt BigInt::operator*(const BigInt& other) const {
    BigInt res;
    res.isNegative = (isNegative != other.isNegative);
    res.digits.resize(digits.size() + other.digits.size(), 0);
    
    for (size_t i = 0; i < digits.size(); ++i) {
        ull carry = 0;
        for (size_t j = 0; j < other.digits.size() || carry; ++j) {
            ull product = res.digits[i + j] + carry;
            if (j < other.digits.size()) {
                product += digits[i] * other.digits[j];
            }
            res.digits[i + j] = product % BASE;
            carry = product / BASE;
        }
    }
    res.remove_leading_zeros();
    if (res.digits.size() == 1 && res.digits[0] == 0) {
        res.isNegative = false;
    }
    return res;
}

BigInt BigInt::operator/(const BigInt& other) const {
    if (other == BigInt(0) || other == BigInt("0")) {
        throw std::invalid_argument("Division by zero");
    }

    BigInt dividend = this->abs();
    BigInt divisor = other.abs();

    BigInt res;
    BigInt current;

    for (int i = dividend.digits.size() - 1; i >= 0; --i) {
        current.digits.insert(current.digits.begin(), dividend.digits[i]);
        current.remove_leading_zeros();

        ll l = 0, r = BASE;
        ll count = 0;

        while (l <= r) {
            ll mid = (l + r) / 2;
            BigInt product = divisor * BigInt(mid);

            if (product <= current) {
                count = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }
        }

        res.digits.insert(res.digits.begin(), count);
        current = current - divisor * BigInt(count);
    }

    res.remove_leading_zeros();

    res.isNegative = (this->isNegative != other.isNegative);

    if (res.digits.empty() || (res.digits.size() == 1 && res.digits[0] == 0)) {
        res.isNegative = false;
    }

    return res;
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

