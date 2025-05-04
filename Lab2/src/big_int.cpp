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
    if (temp.length() == 0) {
        BigInt();
    } else {
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
                digits.push_back(atoi(temp.substr(0, i).c_str()));
            } else {
                digits.push_back(atoi(temp.substr(i - 9, 9).c_str()));
            }
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
    if (isNegative != other.isNegative) {
        return false;
    }
    if (digits.size() != other.digits.size()) {
        return false;
    }
    for (std::size_t i = 0; i < digits.size(); ++i) {
        if (digits[i] != other.digits[i]) {
            return false;
        }
    }
    return true;
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
    std::cout << digits.back() << std::endl << digits.size() << std::endl;
    while (digits.back() == 0 && digits.size() > 1) {
        digits.pop_back();
    }
}

BigInt BigInt::abs() const {
    BigInt temp = BigInt(*this);
    temp.isNegative = false;
    return temp;
}

BigInt BigInt::operator+(const BigInt& other) const {
    BigInt temp = BigInt();
    temp.isNegative = isNegative;
    BigInt abs_cur = this->abs(), abs_other = other.abs();
    if (isNegative && !other.isNegative && abs_cur < abs_other) {
        temp.isNegative = false;
    }
    if (!isNegative && other.isNegative && abs_cur < abs_other) {
        temp.isNegative = true;
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
            temp.digits.push_back(sum % BASE);
            sum /= BASE;
        }
        if (sum > 0) {
            temp.digits.push_back(sum % BASE);
        }
    }
    else {
        if (abs_cur > abs_other) {
            ull diff = 0;
            ull max_size = std::max(digits.size(), other.digits.size());
            ull min_size = std::min(digits.size(), other.digits.size());
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
                temp.digits.push_back(diff);
            }
            for (size_t i = min_size; i < max_size; i++) {
                temp.digits.push_back(diff - in_mind);
                in_mind = 0;
            }
        }
        else {
            ull diff = 0;
            ull max_size = std::max(digits.size(), other.digits.size());
            ull min_size = std::min(digits.size(), other.digits.size());
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
                temp.digits.push_back(diff);
            }
            for (size_t i = min_size; i < max_size; i++) {
                temp.digits.push_back(diff - in_mind);
                in_mind = 0;
            }
        }
    }
    return temp;
}
