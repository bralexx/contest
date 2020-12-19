#include <vector>
#include <iostream>
#include <string>

class BigInteger {
    std::vector<long long> bigint;
    int sign = 1;
    const int bms = 1'000'000'000; //block max size

    long long to_next(const size_t &i) {
        long long tn = bigint[i] / bms;
        if (bigint[i] < 0 && (bigint[i] % bms != 0)) --tn;
        return tn;
    }

    void del_zero_blocks() {
        size_t i = bigint.size() - 1;
        while ((bigint[i] == 0) && (i != 0)) {
            bigint.pop_back();
            --i;
        }
    }

    void update_block(size_t i, int sn = 1) {
        long long tn = to_next(i);
        if (tn == 0) return;
        if (i == bigint.size() - 1) bigint.push_back(sn * tn);
        else bigint[i + 1] += tn * sn;
        bigint[i] = bigint[i] - tn * bms;
    }

    bool static abs_less(const BigInteger &t1, const BigInteger &t2) {
        if (t1.bigint.size() < t2.bigint.size()) return true;
        if (t1.bigint.size() > t2.bigint.size()) return false;
        for (size_t i = t1.bigint.size() - 1;; --i) {
            if (t1.bigint[i] < t2.bigint[i]) return true;
            if (t1.bigint[i] > t2.bigint[i]) return false;
            if (i == 0) return false;
        }
    }


public:
    friend std::istream &operator>>(std::istream &in, BigInteger &bi);

    BigInteger abs() const {
        if (sign == -1)return -(*this);
        return *this;
    }

    BigInteger(const BigInteger &t) {
        sign = t.sign;
        bigint = t.bigint;
    }

    BigInteger(const int &n = 0) {
        bigint.push_back(n);
        if (n < 0) {
            sign = -1;
            bigint[0] *= -1;
        }
        update_block(0);
    }

    ~BigInteger() {
        bigint.clear();
    }

    BigInteger &operator=(const BigInteger &t) {
        sign = t.sign;
        bigint = t.bigint;
        return *this;
    }


    BigInteger &operator+=(const BigInteger &t2) {
        if (sign == -1) {
            sign = 1;
            *this -= t2;
            if (!((bigint.size() == 1) && (bigint[0] == 0))) sign *= -1;
            return *this;
        }
        {
            size_t m = std::max(bigint.size(), t2.bigint.size());
            int s = 1;
            if (t2.sign == -1 && abs_less(*this, t2)) s = -1;
            for (size_t i = 0; i < m; ++i) {
                if (bigint.size() == i) bigint.push_back(0);
                if (i < t2.bigint.size()) {
                    bigint[i] += t2.sign * t2.bigint[i];
                    bigint[i] *= s;
                } else if ((bigint[i] < bms) && (bigint[i] >= 0)) i = m;
                if (i != m)update_block(i, s);
            }
            sign *= s;
            if ((bigint.size() == 1) && (bigint[0] == 0)) sign = 1;
            del_zero_blocks();
        }
        return *this;
    }

    BigInteger &operator-=(const BigInteger &t2) {
        if (sign == -1) {
            sign = 1;
            *this += t2;
            if (!((bigint.size() == 1) && (bigint[0] == 0))) sign *= -1;
            return *this;
        }
        size_t m = std::max(bigint.size(), t2.bigint.size());
        int s = 1;
        if (t2.sign == 1 && abs_less(*this, t2)) s = -1;
        for (size_t i = 0; i < m; ++i) {
            if (bigint.size() == i) bigint.push_back(0);
            if (i < t2.bigint.size()) {
                bigint[i] -= t2.sign * t2.bigint[i];
                bigint[i] *= s;
                update_block(i, s);
            } else if ((bigint[i] < bms) && (bigint[i] >= 0)) i = m;
            if (i != m)update_block(i, s);
        }
        sign *= s;
        if ((bigint.size() == 1) && (bigint[0] == 0)) sign = 1;
        del_zero_blocks();
        return *this;
    }

    BigInteger &operator*=(const BigInteger& t) {
        BigInteger this0(0);
        this0 = *this;
        *this = BigInteger(0);
        size_t sz = t.bigint.size();
        for (size_t i = 0; i < sz; ++i) {
            for (size_t j = 0; j < this0.bigint.size(); ++j) {
                if (j + i == bigint.size()) bigint.push_back(0);
                bigint[j + i] += this0.bigint[j] * t.bigint[i];
                update_block(j + i);
            }
        }
        del_zero_blocks();
        sign = this0.sign * t.sign;
        if ((bigint.size() == 1) && (bigint[0] == 0)) sign = 1;
        return *this;
    }

    BigInteger &operator/=(const BigInteger &div) {
        int s = sign * div.sign;
        BigInteger answer(0);
        BigInteger num(0);
        bool exit = false;
        for (size_t i = bigint.size() - 1; !exit; --i) {
            int l = 0, r = bms;
            num *= bms;
            num += bigint[i];
            num.del_zero_blocks();
            if (!abs_less(num, div)) {
                int mid;
                while (r - l > 1) {
                    mid = (r + l) / 2;
                    if (abs_less(num, div * mid)) r = mid;
                    else l = mid;
                }
                num -= div.abs() * l;
            }
            answer *= bms;
            answer += l;
            if (i == 0) exit = true;
        }
        *this = answer;
        //del_zero_blocks();
        sign = s;
        if ((bigint.size() == 1) && (bigint[0] == 0)) sign = 1;
        return *this;
    }

    BigInteger &operator%=(const BigInteger &div) {
        BigInteger copy = *this;
        copy /= div;
        copy *= div;
        *this -= copy;
        //if(*this < 0) *this += div;
        return *this;
    }

    BigInteger operator*(const BigInteger &t2) const {
        BigInteger copy(0);
        copy = *this;
        copy *= t2;
        return copy;
    }

    BigInteger operator-() const {
        BigInteger copy = *this;
        if ((bigint.size() == 1) && (bigint[0] == 0)) return copy;
        copy.sign *= -1;
        return copy;
    }

    BigInteger &operator++() {
        *this += 1;
        return *this;
    }

    BigInteger operator++(int) {
        BigInteger copy = *this;
        ++(*this);
        return copy;
    }

    BigInteger &operator--() {
        *this -= 1;
        return *this;
    }

    BigInteger operator--(int) {
        BigInteger copy = *this;
        --(*this);
        return copy;
    }

    bool operator==(const BigInteger &t) const {
        if (sign != t.sign) return false;
        if (bigint.size() != t.bigint.size()) return false;
        for (size_t i = 0; i < bigint.size(); ++i) {
            if (bigint[i] != t.bigint[i]) return false;
        }
        return true;
    }

    bool operator<(const BigInteger &t) const {
        if (sign < t.sign) return true;
        if (sign > t.sign) return false;
        if (sign == 1) return abs_less(*this, t);
        if (sign == -1) return abs_less(t, *this);
        return false;
    }

    bool operator>(const BigInteger &t) const {
        return !((*this == t) || (*this < t));
    }

    bool operator<=(const BigInteger &t) const {
        return !(*this > t);
    }

    bool operator>=(const BigInteger &t) const {
        return !(*this < t);
    }

    bool operator!=(const BigInteger &t) const {
        return !(*this == t);
    }

    std::string toString() const {
        std::string str;
        str = "";
        std::string str2;
        if (sign == -1 && ((bigint.size() != 1) || (bigint[0] != 0))) str += '-';
        str += std::to_string(bigint[bigint.size() - 1]);
        for (int i = (int) bigint.size() - 2; i >= 0; --i) {
            str2 = std::to_string(bigint[i]);
            for (int j = 0; j < 9 - (int) str2.length(); ++j) str += '0';
            str += str2;
        }
        return str;
    }

    explicit operator bool() const {
        if ((*this) == 0) {
            return false;
        }
        return true;
    }
};

BigInteger operator+(const BigInteger &t1, const BigInteger &t2) {
    BigInteger copy = t1;
    copy += t2;
    return copy;
}

BigInteger operator-(const BigInteger &t1, const BigInteger &t2) {
    BigInteger copy = t1;
    copy -= t2;
    return copy;
}

BigInteger operator/(const BigInteger &t1, const BigInteger &t2) {
    BigInteger copy = t1;
    copy /= t2;
    return copy;
}

BigInteger operator%(const BigInteger &t1, const BigInteger &t2) {
    BigInteger copy = t1;
    copy %= t2;
    return copy;
}

BigInteger operator*(const int &t1, const BigInteger &t2) {
    BigInteger copy(t1);
    copy *= t2;
    return copy;
}

std::ostream &operator<<(std::ostream &out, const BigInteger &bi) {
    out << bi.toString();
    return out;
}

std::istream &operator>>(std::istream &in, BigInteger &bi) {
    bi.bigint.clear();
    std::string str;
    in >> str;
    int i = 0;
    if (str[0] == '-') {
        i = 1;
        bi.sign = -1;
    } else bi.sign = 1;
    long long num;
    for (int j = (int) str.length() - 1; j >= i; j -= 9) {
        num = 0;
        if (j + 1 - i >= 9)
            for (int k = 8; k >= 0; --k) {
                num *= 10;
                num += str[j - k] - '0';
            }
        else
            for (; i <= j; ++i) {
                num *= 10;
                num += str[i] - '0';
            }
        bi.bigint.push_back(num);
    }
    bi.del_zero_blocks();
    if ((bi.bigint.size() == 1) && (bi.bigint[0] == 0)) bi.sign = 1;
    return in;
}

BigInteger operator "" _bi(const unsigned long long t) {
    return BigInteger((int) t);
}

class Rational {
    BigInteger num;
    BigInteger den = 1;

    void abs_den() {
        if (den < 0) {
            num *= -1;
            den *= -1;
        }
    }

    BigInteger nod(const BigInteger &t1, const BigInteger &t2) {
        BigInteger mod = t1 % t2;
        if (mod == 0) return t2;
        return nod(t2, mod);
    }

    void reduce() {
        BigInteger nod_num_den;
        if (num.abs() < den) nod_num_den = nod(den, num);
        else nod_num_den = nod(num, den);
        num /= nod_num_den;
        den /= nod_num_den;
        abs_den();
    }

public:
    Rational(const BigInteger &bi) {
        num = bi;
    }

    Rational(const int &t = 0) {
        num = t;
        den = 1;
    }

    Rational &operator+=(const Rational &t) {
        BigInteger num2 = t.num * den;
        den *= t.den;
        num *= t.den;
        num += num2;
        reduce();
        return *this;
    }

    Rational &operator-=(const Rational &t) {
        BigInteger num2 = t.num * den;
        den *= t.den;
        num *= t.den;
        num -= num2;
        reduce();
        return *this;
    }

    Rational &operator*=(const Rational &t) {
        den *= t.den;
        num *= t.num;
        reduce();
        return *this;
    }

    Rational &operator/=(const Rational &t) {
        den *= t.num;
        num *= t.den;
        abs_den();
        reduce();
        return *this;
    }

    Rational operator-() {
        return (num * (-1));
    }

    bool operator<(const Rational &t2) const {
        if (num < 0 && t2.num > 0) return true;
        if (num > 0 && t2.num < 0) return false;
        if (den == t2.den) return num < t2.num;
        return (num * t2.den) < (t2.num * den);
    }

    bool operator==(const Rational &t2) const {
        return (num == t2.num) && (den == t2.den);
    }

    bool operator>(const Rational &t2) const {
        return !(*this < t2 && *this == t2);
    }

    bool operator<=(const Rational &t2) const {
        return !(*this > t2);
    }

    bool operator>=(const Rational &t2) const {
        return !(*this < t2);
    }

    bool operator!=(const Rational &t2) const {
        return !(*this == t2);
    }

    std::string toString() const {
        std::string str = num.toString();
        if (den != 1) {
            str += '/';
            str += den.toString();
        }
        return str;
    }

    std::string asDecimal(size_t precision = 0) const {
        if (den == 1) return num.toString();
        std::string result;
        result = "";
        BigInteger dec = num / den;
        if (num < 0) {
            result += '-';
            dec = dec.abs();
        }
        result += dec.toString();
        dec = num.abs() % den;
        if ((precision != 0) && (dec != 0)) {
            result += '.';
            size_t i = 0;
            if (precision > 9) for (; i < precision - 9; i += 9) dec *= 1'000'000'000;
            for (; i < precision; ++i) dec *= 10;
            dec /= den;
            std::string str = dec.toString();
            for (i = 0; i < (precision - str.length()); ++i) result += '0';
            result += str;
            for (i = result.length() - 1; result[i] == '0'; --i) result.pop_back();
        }
        return result;
    }

    /*explicit operator double const() {
        std::string str = this -> asDecimal(1000);
        double d = 0;
    }*/
};

Rational operator+(const Rational &t1, const Rational &t2) {
    Rational result = t1;
    result += t2;
    return result;
}

Rational operator-(const Rational &t1, const Rational &t2) {
    Rational result = t1;
    result -= t2;
    return result;
}

Rational operator*(const Rational &t1, const Rational &t2) {
    Rational result = t1;
    result *= t2;
    return result;
}

Rational operator/(const Rational &t1, const Rational &t2) {
    Rational result = t1;
    result /= t2;
    return result;
}

#ifndef BIGINTEGER_BIGINTEGER_H
#define BIGINTEGER_BIGINTEGER_H

#endif //BIGINTEGER_BIGINTEGER_H
