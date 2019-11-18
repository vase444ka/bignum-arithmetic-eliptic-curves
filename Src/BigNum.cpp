#include <BigNum.hpp>


namespace {
/**
 * @brief Points to the max value BigNum array's cell can hold,
 *        same as basis in linear representation
 */
constexpr int NUM_BASE = 1000000000;

/**
 * @brief Points to number of digits in (NUM_BASE-1)
 */
constexpr char SECTION_DIGITS = 9;
}

namespace lab {

BigNum::BigNum(std::string_view num_str) {
    for (int i = num_str.size() - 1; i >= 0; i -= SECTION_DIGITS) {
        std::string curr_section;
        if (i - SECTION_DIGITS + 1 < 0) {
            curr_section = num_str.substr(0, SECTION_DIGITS + (i - SECTION_DIGITS + 1));
        } else {
            curr_section = num_str.substr(i - SECTION_DIGITS + 1, SECTION_DIGITS);
        }
        _digits.push_back(std::stoi(curr_section));
    }
    while (!_digits.empty() && _digits.back() == 0) {
        _digits.pop_back();
    }
}

std::string to_string(const BigNum &num)
{
    std::string result;
    int leading_zeros = 0;

    for (int curr_pos = 0; curr_pos < num._digits.size(); ++curr_pos) {
        std::string temp = std::to_string(num._digits[curr_pos]);
        std::string offset;
        leading_zeros = 0;
        while (temp.size() + offset.size() != SECTION_DIGITS) {
            offset += '0';
            ++leading_zeros;
        }
        result = offset + temp + result;
    }

    result.erase(0, leading_zeros);
    return result;
}

bool operator<(const BigNum &left, const BigNum &right) {
    if (left._digits.size() < right._digits.size()) return true;
    if (left._digits.size() > right._digits.size()) return false;
    for (int curr_pos = left._digits.size() - 1; curr_pos >= 0; --curr_pos) {
        if (left._digits[curr_pos] < right._digits[curr_pos]) return true;
        if (left._digits[curr_pos] > right._digits[curr_pos]) return false;
    }
    return false;
}

bool operator>(const BigNum &left, const BigNum &right) {
    return (right < left);
}

bool operator<=(const BigNum &left, const BigNum &right) {
    return !(right < left);
}

bool operator>=(const BigNum &left, const BigNum &right) {
    return !(left < right);
}

bool operator==(const BigNum &left, const BigNum &right) {
    return ((!(right < left)) && (!(left < right)));
}

bool operator!=(const BigNum &left, const BigNum &right) {
    return !(left == right);
}

BigNum operator+(const BigNum &left, const BigNum &right) {
    BigNum result;
    if (right._digits.size() > left._digits.size()) {
        result = right;
        int addition = 0;
        for (int curr_pos = 0; curr_pos < left._digits.size(); ++curr_pos) {
            result._digits[curr_pos] += left._digits[curr_pos] + addition;
            addition = result._digits[curr_pos] / NUM_BASE;
            result._digits[curr_pos] = result._digits[curr_pos] % NUM_BASE;
        }
    } else {
        result = left;
        int addition = 0;
        for (int curr_pos = 0; curr_pos < right._digits.size(); ++curr_pos) {
            long long temp = static_cast<long long>(result._digits[curr_pos])
                    + static_cast<long long>(right._digits[curr_pos]) + addition;
            result._digits[curr_pos] = temp % NUM_BASE;
            addition = temp / NUM_BASE;
            result._digits[curr_pos] = result._digits[curr_pos] % NUM_BASE;
        }
    }
    return result;
}

BigNum operator-(const BigNum &left, const BigNum &right) {
    BigNum result = left;
    for (int curr_pos = 0; curr_pos < right._digits.size(); ++curr_pos) {
        if (result._digits[curr_pos] < right._digits[curr_pos]) {
            result._digits[curr_pos] = result._digits[curr_pos] - right._digits[curr_pos] + NUM_BASE;
            result._digits[curr_pos + 1] -= 1;
        }
        else {
            result._digits[curr_pos] = result._digits[curr_pos] - right._digits[curr_pos];
        }
    }
    if (left._digits.size() > right._digits.size()) {
        if (result._digits[right._digits.size()] < 0) {
            result._digits[right._digits.size()] += NUM_BASE;
            result._digits[right._digits.size() + 1] -= 1;
        }
    }
    return result;
}

std::vector<char> toOneDigit(const BigNum &num) {
    std::vector<char> fnum;
    for (int curr_pos = 0; curr_pos < num._digits.size(); ++curr_pos) {
        int counter = num._digits[curr_pos];
        for (int i = 0; i < SECTION_DIGITS; ++i) {
            fnum.push_back(counter % 10);
            counter = counter / 10;
        }
    }
    while (!fnum.empty() && fnum.back() == 0) {
        fnum.pop_back();
    }
    return fnum;
}

BigNum from_string(std::string_view str)
{
    return BigNum(str);
}

BigNum toBigNum(std::vector<char> &num_digits) {
    std::string str_num;
    for (int i = num_digits.size() - 1; i >= 0; --i) {
        str_num += ('0' + num_digits[i]);
    }
    BigNum result(str_num);
    return result;
}

BigNum operator*(const BigNum &left, int right) {
    BigNum result = left;
    unsigned long long addition = 0;
    for (int curr_pos = 0; curr_pos < result._digits.size(); ++curr_pos) {
        unsigned long long temp = static_cast<long long>(result._digits[curr_pos]) * right + addition;
        result._digits[curr_pos] = temp % NUM_BASE;
        addition = temp / NUM_BASE;
    }
    if (addition >= NUM_BASE) {
        result._digits.push_back(addition % NUM_BASE);
        result._digits.push_back(addition / NUM_BASE);
    }
    else if (addition > 0 && addition < NUM_BASE) {
        result._digits.push_back(addition);
    }
    return result;
}

bool isLowerDigits(const std::vector<char> &left, const std::vector<char> &right, int step);

auto addDigits (const std::vector <char> &lhs, const std::vector <char> &rhs) {

    constexpr auto BASE = 10;
    std::vector <char> result;


    if (isLowerDigits(lhs, rhs, 0)) {
        result = rhs;
        int addition{};
        for (std::size_t i = 0; i < rhs.size(); ++i) {
            result[i] += (i >= lhs.size()) ? addition : lhs[i] + addition;
            addition = result[i] / BASE;
            result[i] %= BASE;
        }
    }
    else {
        result = lhs;
        int addition{};
        for (std::size_t i = 0; i < rhs.size(); ++i) {
            result[i] += (i >= rhs.size()) ? addition : rhs[i] + addition;
            addition = result[i] / BASE;
            result[i] %= BASE;
        }
    }
    return result;

}

namespace {

    inline std::vector<int> naiveMultipl (const std::vector<int>& lhs, const std::vector<int>& rhs) {

        std::vector <int> result (lhs.size() + rhs.size());

        for (int i = 0; i < lhs.size(); ++i){
            uint64_t addition{};
            for (int j = 0; j < rhs.size(); ++j) {
                uint64_t temp = static_cast<uint64_t>(lhs[i]) * rhs[j] + addition + result[i + j];
                addition = temp / NUM_BASE;
                result[i + j] = temp % NUM_BASE;
            }
            result [i + rhs.size()] += addition;
        }
        if (result.back() >= NUM_BASE) {
            auto temp = result.back();
            result.back() = temp % NUM_BASE;
            result.push_back(temp / NUM_BASE);
        }

        if (result.back() == 0)
            result.pop_back();

        return result;
    }

    inline std::vector<int> addVectors (const std::vector<int>& lhs, const std::vector<int>& rhs) {

        const auto length = lhs.size();
        std::vector<int> result(length + 1);
        int addition {};

        for (int i = 0; i < length; ++i) {
            result[i] += lhs[i] + rhs[i];
            addition = result[i] / NUM_BASE;
            result[i] %= NUM_BASE;
        }
        result[length] += addition;
        return result;
    }

    inline std::vector<int> substractVectors (const std::vector<int>& lhs, const std::vector<int>& rhs) {

        auto result = lhs;

        for (int i = 0; i < rhs.size(); ++i) {
            if (result[i] < rhs[i]) {
                result[i] = result[i] - rhs[i] + NUM_BASE;
                result[i + 1] -= 1;
            } else {
                result[i] -= - rhs[i];
            }
        }
        if (lhs.size() > rhs.size()) {
            if (result[rhs.size()] < 0) {
                result[rhs.size()] += NUM_BASE;
                result[rhs.size() + 1] -= 1;
            }
        }
        return result;
    }

    /**
     * @brief Minimum size of vector of digits to do
     *        fast multiplication instead of naive approach
     */

    constexpr inline int MIN_FOR_KARATSUBA = 10;

    /*
     * @brief Karatsuba's method implements fast multiplication of numbers [AB] and [CD] like
     *        like (A * 10 + B) * (C * 10 + D) = AC * 100 + BD + ((A + B) * (C + D) - AC - BD) * 10
     */

    std::vector <int> karatsuba (std::vector <int> lhs, std::vector<int> rhs) {

        if (lhs.size() <= MIN_FOR_KARATSUBA || rhs.size() <= MIN_FOR_KARATSUBA)
            return naiveMultipl(lhs, rhs);

        while (lhs.size() > rhs.size())
            rhs.push_back(0);
        while (rhs.size() > lhs.size())
            lhs.push_back(0);

        const auto length = lhs.size();
        std::vector <int> result (length * 2);

        std::vector <int> lhsL (lhs.begin() + length / 2, lhs.end());
        std::vector <int> rhsL (rhs.begin() + length / 2, rhs.end());
        std::vector <int> lhsR (lhs.begin(), lhs.begin() + length / 2);
        std::vector <int> rhsR (rhs.begin(), rhs.begin() + length / 2);

        auto c1 = karatsuba(lhsL, rhsL);
        auto c2 = karatsuba(lhsR, rhsR);

        std::vector <int> lhsLR = addVectors(lhsL, lhsR);
        std::vector <int> rhsLR = addVectors(rhsL, rhsR);

        auto c3 = karatsuba(lhsLR, rhsLR);

        c3 = substractVectors(c3, addVectors(c2, c1));
        std::cout << std::endl;
//        for (auto & i : c3)
//            std::cout << i << ' ' << std::flush;

        c2.resize(length * 2);
        result = std::move(c2);

        for (auto i = length; i < length * 2; ++i)
            result[i] = c1[i - length];

        int addition{};
        for (auto i = length / 2; i < length + length / 2; ++i) {
            uint64_t temp = c3[i - length / 2] + addition + result[i];
            addition = temp / NUM_BASE;
            result[i] = temp % NUM_BASE;
        }

        result[length + length / 2] += addition;
        if (result[length + length / 2] > NUM_BASE) {
            result [length + length / 2 + 1] += result[length + length / 2] / NUM_BASE;
            result [length + length / 2] %= NUM_BASE;
        }

        while (result.back() == 0)
            result.pop_back();

        return result;
    }
}


BigNum operator* (const BigNum &lhs, const BigNum &rhs) {

    BigNum result;
    result._digits = karatsuba(lhs._digits, rhs._digits);
    return result;
}

/**
 * @brief Compares vector of digits with operator <
 * @param step points to the number of digits from beginning of left var to compare
 */
bool isLowerDigits(const std::vector<char> &left, const std::vector<char> &right, int step) {
    if ((step > right.size()) && (step <= left.size())) return false;
    if ((step < right.size()) || (left.size() < right.size())) return true;
    if (step > left.size()) return true;
    for (int i = right.size() - 1; i >= 0; --i) {
        if (left[left.size() + i - step] < right[i]) return true;
        if (left[left.size() + i - step] > right[i]) return false;
    }
    return false;
}

/**
 * @brief Compares vector of digits with operator >=
 * @param step points to the number of digits from beginning of left var to compare
 */
bool isBiggerDigits(std::vector<char> &num1, std::vector<char> &num2, int step) {
    return !(isLowerDigits(num1, num2, step));
}

/**
 * @brief Multiplies vector of digits and int
 */
std::vector<char> multiplyDigits(std::vector<char> &num1, int num2) {
    std::vector<char> result = num1;
    int addition = 0;
    for (int curr_pos = 0; curr_pos < result.size(); ++curr_pos) {
        long long temp = result[curr_pos] * num2 + addition;
        result[curr_pos] = temp % 10;
        addition = temp / 10;
    }
    if (addition != 0) {
        result.push_back(addition);
    }
    return result;
}

/**
 * @brief Subtracts vectors of digits,
 *        where the second number will be aligned to beginning of the first
 * @param side points to number of digits from beginning of left var to delete
 */
void leftSubstract(std::vector<char> &num1, std::vector<char> &num2, int side) {
    for (int i = 0; i < num2.size(); ++i) {
        if (num1[num1.size() - side + i] < num2[i]) {
            num1[num1.size() - side + i] = 10 + num1[num1.size() - side + i] - num2[i];
            num1[num1.size() - side + i + 1] -= 1;
        } else {
            num1[num1.size() - side + i] = num1[num1.size() - side + i] - num2[i];
        }
    }
}

std::pair<BigNum, BigNum> extract(const BigNum &left, const BigNum &right) {
    if (left < right)
        return std::pair<BigNum, BigNum>(BigNum("0"), left);
    std::vector<char> fnum = toOneDigit(left);
    std::vector<char> snum = toOneDigit(right);
    std::vector<char> result;
    std::vector<char> current = snum;
    std::vector<char> temp;
    int side = 1;
    int power = 1;

    side = current.size();
    while ((isLowerDigits(fnum, current, side))) {
        side++;
    }
    while (true) {
        if ((power == 1) && (isLowerDigits(fnum, current, side))) {
            side++;
            if (side > fnum.size()) {
                break;
            }
            result.push_back(0);
        }
        if (isBiggerDigits(fnum, current, side)) {
            power++;
            current = multiplyDigits(snum, power);
        }
        if ((power != 1) && (isLowerDigits(fnum, current, side))) {
            current = multiplyDigits(snum, power - 1);
            leftSubstract(fnum, current, side);
            int prev_size = temp.size();
            temp.clear();
            while (!fnum.empty() && fnum.back() == 0) {
                fnum.pop_back();
                side--;
            }
            result.push_back(power - 1);
            power = 1;
            current = snum;
            side++;
        }
    }
    std::reverse(result.begin(), result.end());
    return std::pair<BigNum, BigNum>(toBigNum(result), toBigNum(fnum));
}

void modify(BigNum &num, const BigNum &mod) {
    num = extract(num, mod).second;
}

BigNum add(const BigNum &left, const BigNum &right, const BigNum &mod) {
    auto t_num2 = right % mod;
    auto t_num1 = left % mod;
    BigNum result = t_num1 + t_num2;
    modify(result, mod);
    return result;
}

BigNum subtract(const BigNum &left, const BigNum &right, const BigNum &mod) {
    auto t_num2 = right % mod;
    auto t_num1 = left % mod;
    BigNum result;
    if (t_num1 >= t_num2) {
        result = t_num1 - t_num2;
    } else {
        result = mod - t_num2 + t_num1;
    }
    return result;
}

BigNum operator%(const BigNum &left, const BigNum &right) {
    BigNum result = extract(left, right).second;
    return result;
}
}