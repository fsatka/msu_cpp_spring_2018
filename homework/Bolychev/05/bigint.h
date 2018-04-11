#ifndef BIGINT_BIGINT_H
#define BIGINT_BIGINT_H

#include <iostream>
#include <stdexcept>

typedef int8_t digit;

class BigInt {
private:
    const digit operator[](size_t i) const {
        if (i >= length or *this == BigInt(0)) {
            return 0;
        }

        return number[length - 1 - i];
    }

    void fitSize()
    {
        auto buffer = new digit[length];
        std::copy(number, number + length, buffer);

        delete []number;
        number = buffer;
    }

    void getRidOfNulls()
    {
        if (*number == 0) {
            // Посчитаем ведущие нули в начале
            size_t nulls_counter ;
            for (nulls_counter = 0; nulls_counter < length; ++nulls_counter)
            {
                if (number[nulls_counter] != 0) {
                    break;
                }
            }

            digit* memory_for_num;
            if (nulls_counter == length) {
                memory_for_num = new digit[1];
                *memory_for_num = 0;
                length = 1;
                is_negative = false;
            } else {
                memory_for_num = new digit[length - nulls_counter];
                std::copy(number + nulls_counter, number + length, memory_for_num);
                length = length - nulls_counter;
            }

            delete [] number;
            number = memory_for_num;
        }
    }

    bool is_abs_bigger(const BigInt& other) const // return *this > other
    {
        if (length == 0 || other.length == 0) {
            return false;
        }

        if (length > other.length) {
            return true;
        } else if (length < other.length) {
            return false;
        }

        for (size_t i = 0; i < length; ++i) {
            if (number[i] > other.number[i]) {
                return true;
            } else if (number[i] < other.number[i]) {
                return false;
            }
        }

        return false;
    }

    BigInt subtractAbsValues(const BigInt& bigger,
                             const BigInt& smaller,
                             const bool is_result_negative = false) const // |bigger| - |smaller|
    {
        BigInt result(0, bigger.length, is_result_negative);
        for (int i = bigger.length - 1; i >= 0; --i)
        {
            result.number[result.length - 1 - i] = smaller.length >= i + 1 ?
                                                   bigger.number[bigger.length - 1 - i] -
                                                   smaller.number[smaller.length - 1 - i] :
                                                   bigger.number[bigger.length - 1 - i];

        }

        for (int i = result.length - 1; i > 0; --i) {
            if (result.number[i] < 0) {
                result.number[i] += 10;
                result.number[i - 1]--;
            }
        }


        result.getRidOfNulls();

        return result;
    }

    BigInt sumAbsValues(const BigInt& x, const BigInt& y, const bool is_result_neg = false) const // |x| + |y|
    {
        bool max_condition = x.length > y.length;

        size_t max_len = max_condition ? x.length : y.length;
        BigInt result(0, 1 + max_len, is_result_neg);

        auto n_max = max_condition ? x: y;
        auto n_min = max_condition ? y: x;
        for (size_t i = 0; i < max_len; ++i)
        {
            result.number[result.length - 1 - i] =
                    n_min.length >= i + 1 ? n_max.number[n_max.length - 1 - i] +
                                            n_min.number[n_min.length - 1 - i]
                                          : n_max.number[n_max.length - 1 - i];
        }

        for (size_t i = result.length - 1; i > 0; --i)
        {
            if (result.number[i] >= 10) {
                result.number[i] -= 10;
                ++result.number[i - 1];
            }
        }

        result.getRidOfNulls();

        return result;
    }

    BigInt multiplyOnDigit(const BigInt& number, const digit multiply_digit, const size_t nulls_to_end) const
    {
        BigInt result(0, number.length + nulls_to_end + 1, false);

        auto j = result.length - nulls_to_end - 1;
        for (int i = number.length - 1; i >= 0; --i) {
            result.number[j--] = number.number[i] * multiply_digit;
        }

        for (int i = result.length - nulls_to_end - 1; i > 0; --i) {
            result.number[i - 1] += result.number[i] / 10;
            result.number[i] = result.number[i] % 10;
        }

        result.getRidOfNulls();

        return result;
    }

    BigInt cutDividendToDivisor(const BigInt& dividend, const BigInt& divisor) const
    {
        BigInt result(dividend);

        result.length = divisor.length;

        if (result < divisor) {
            result.length++;
        }

        return result;
    }

    digit* number;
    bool is_negative;
    size_t length; // if negative "-" doesn't count
public:

    BigInt() : is_negative{false}, length{1}, number{new digit[1]} {
        *number = 0;
    }

    BigInt(const digit filling_value, const size_t size, const bool is_neg) : length{size},
                                                                   number{new digit[size]},
                                                                   is_negative{is_neg} {
        for (size_t i = 0; i < length; ++i) {
            number[i] = filling_value;
        }
    }

    BigInt(const int64_t input_number) {
        if (input_number == 0) {
            is_negative = false;
            number = new digit[1];
            *number = 0;
            length = 1;
            return;
        } else {
            is_negative = input_number < 0;
        }

        digit buffer[30];
        auto abs_number = (input_number >= 0? input_number : -input_number);
        length = 0;
        while (abs_number > 0) {
            buffer[length++] = abs_number % 10;
            abs_number /= 10;
        }

        number = new digit[length];
        for (int i = 0; i < length; ++i) {
            number[i] = buffer[length - i - 1];
        }
    }

    ~BigInt() {
        delete [] number;
    }

    BigInt(const BigInt& copied) {
        length = copied.length;
        number = new digit[length];
        is_negative = copied.is_negative;

        for (size_t i = 0; i < length; ++i) {
            number[i] = copied.number[i];
        }
    }

    bool operator<(const BigInt& other) const
    {
        if (is_negative && !other.is_negative) {
            return true;
        } else if (!is_negative && other.is_negative) {
            return false;
        } else if (is_negative && other.is_negative) {
            return is_abs_bigger(other);
        }
        return other.is_abs_bigger(*this);

    }

    bool operator>(const BigInt& other) const
    {
        return other<(*this);
    }

    bool operator<=(const BigInt& other) const {
        return !(*this > other);
    }

    bool operator>=(const BigInt& other) const {
        return !(*this < other);
    }

    BigInt operator-() const {
        BigInt res = BigInt(*this);
        if (res == BigInt(0)) {
            return res;
        }

        res.is_negative = !is_negative;
        return res;
    }

    BigInt& operator=(const BigInt& copied)
    {
        if (number == copied.number) {
            return *this;
        }

        delete[] number;

        length = copied.length;
        number = new digit[length];
        is_negative = copied.is_negative;

        for (size_t i = 0; i < length; ++i) {
            number[i] = copied.number[i];
        }

        return *this;
    }

    BigInt operator+(const BigInt& other) const
    {
        if (is_negative && other.is_negative) {
            bool result_is_negative = true;
            return sumAbsValues(*this, other, result_is_negative);
        } else if (!is_negative && !other.is_negative) {
            return sumAbsValues(*this, other);
        } else if (!is_negative && other.is_negative) {
            if (other.is_abs_bigger(*this)) {
                bool result_is_negative = true;
                return subtractAbsValues(other, *this, result_is_negative);
            } else {
                return subtractAbsValues(*this, other);
            }
        } else { // (is_negative && !other.is_negative)
            if (is_abs_bigger(other)) {
                bool result_is_negative = true;
                return subtractAbsValues(*this, other, result_is_negative);
            } else {
                return subtractAbsValues(other, *this);
            }
        }
    }

    BigInt operator-(const BigInt& other) const
    {
        // *this - other == *this + (-other) =>
        // other.is_negative changing to !other.is_negative

        if (is_negative && !other.is_negative) {
            bool result_is_negative = true;
            return sumAbsValues(*this, other, result_is_negative);
        } else if (!is_negative && other.is_negative) {
            return sumAbsValues(*this, other);
        } else if (!is_negative && !other.is_negative) {
            if (other.is_abs_bigger(*this)) {
                bool result_is_negative = true;
                return subtractAbsValues(other, *this, result_is_negative);
            } else {
                return subtractAbsValues(*this, other);
            }
        } else { // (is_negative && other.is_negative)
            if (is_abs_bigger(other)) {
                bool result_is_negative = true;
                return subtractAbsValues(*this, other, result_is_negative);
            } else {
                return subtractAbsValues(other, *this);
            }
        }
    }

    BigInt operator*(const BigInt& other) const
    {
        BigInt nullValue(0);

        if (*this == nullValue || other == nullValue) {
            return nullValue;
        }

        BigInt result(0, length + other.length, is_negative ^ other.is_negative);

        BigInt* mult_items = new BigInt[other.length];
        for (size_t i = 0; i < other.length; ++i) {
            mult_items[i] = multiplyOnDigit(*this, other[i], i);
        }

        for (size_t j = 0; j < mult_items[other.length-1].length; ++j)
        {
            for (size_t i = 0; i < other.length; ++i) {
                result.number[result.length - 1 - j] += mult_items[i][j];
                if (result.number[result.length - 1 - j] > 100) {
                    result.number[result.length - 1 - j - 1] += result.number[result.length - 1 - j] / 10;
                    result.number[result.length - 1 - j] = result.number[result.length - 1 - j] % 10;
                }
            }
        }

        for (int i = result.length - 1; i > 0; --i) {
            result.number[i - 1] += result.number[i] / 10;
            result.number[i] = result.number[i] % 10;
        }

        result.getRidOfNulls();

        return result;
    }

    BigInt operator/(const BigInt& other) const
    {
        if (other == BigInt(0)) {
            throw std::out_of_range("Division on zero");
        }

        if (other.is_abs_bigger(*this)) {
            return BigInt(0);
        }

        BigInt residual;
        BigInt dividend(*this);
        dividend.is_negative = false;
        BigInt divisor(other);
        divisor.is_negative = false;

        BigInt result(0, dividend.length, is_negative ^ other.is_negative);
        result.length = 0;

        auto cut = cutDividendToDivisor(dividend, divisor);
        while(true)
        {
            int i;
            for (i = 0; true; ++i) {
                residual = cut - BigInt(i) * divisor;
                if (residual < divisor) {
                    break;
                }
            }

            result.number[result.length++] = i;

            if (dividend.length == cut.length) {
                break;
            }

            if (residual > 0) {
                auto memory_for_dividend = new digit[residual.length + dividend.length - cut.length];
                std::copy(residual.number, residual.number + residual.length, memory_for_dividend);
                std::copy(dividend.number + cut.length, dividend.number + length,
                          memory_for_dividend + residual.length);
                dividend.length = residual.length + dividend.length - cut.length;
                delete[] dividend.number;
                dividend.number = memory_for_dividend;
                cut.length = residual.length + 1;
            } else {
                auto memory_for_dividend = new digit[dividend.length - cut.length];
                std::copy(dividend.number + cut.length, dividend.number + length,
                          memory_for_dividend);
                dividend.length = dividend.length - cut.length;
                delete[] dividend.number;
                dividend.number = memory_for_dividend;
                cut.length = 1;
            }

            std::copy(dividend.number, dividend.number + cut.length, cut.number);
        }

        result.fitSize();

        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const BigInt& number)
    {
        if (number.is_negative) {
            os << "-";
        }

        for (size_t i  = 0; i < number.length; ++i) {
            os << static_cast<int>(number.number[i]);
        }

        return os;
    }

    bool operator==(const BigInt& other) const
    {
        if (length != other.length || is_negative != other.is_negative) {
            return false;
        }

        for (size_t i = 0; i < length; ++i) {
            if (number[i] != other.number[i]) {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const BigInt& other) const {
        return !(*this == other);
    }
};


#endif //BIGINT_BIGINT_H
