#include "Polynomial.h"

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>

static constexpr double EPS = 1e-10;
static constexpr int MAX_EXPONENT = 1000;

// 去掉字符串首尾空白字符。
static std::string trim_copy(const std::string &s) {
    std::size_t first = 0;
    while (first < s.size() && std::isspace(static_cast<unsigned char>(s[first]))) {
        ++first;
    }
    std::size_t last = s.size();
    while (last > first && std::isspace(static_cast<unsigned char>(s[last - 1]))) {
        --last;
    }
    return s.substr(first, last - first);
}

// 从一行开头解析 double，解析失败返回 false。
static bool parse_double_prefix(const std::string &line, double &value) {
    std::string t = trim_copy(line);
    if (t.empty()) return false;
    char *end = nullptr;
    value = std::strtod(t.c_str(), &end);
    return end != t.c_str() && std::isfinite(value);
}

// 从一行开头解析 int，解析失败返回 false。
static bool parse_int_prefix(const std::string &line, int &value) {
    std::string t = trim_copy(line);
    if (t.empty()) return false;
    char *end = nullptr;
    long parsed = std::strtol(t.c_str(), &end, 10);
    if (end == t.c_str()) return false;
    if (parsed < std::numeric_limits<int>::min() || parsed > std::numeric_limits<int>::max()) {
        return false;
    }
    value = static_cast<int>(parsed);
    return true;
}

// 循环读取系数，直到输入合法或输入流结束。
static bool read_coefficient(std::istream &in, std::ostream &out, double &coefficient) {
    std::string line;
    while (true) {
        out << "coefficient? ";
        if (!std::getline(in, line)) return false;
        if (parse_double_prefix(line, coefficient)) return true;
        out << "coefficient must be double!" << std::endl;
    }
}

// 循环读取指数，并限制指数范围为 0 到 1000。
static bool read_exponent(std::istream &in, std::ostream &out, int &exponent) {
    std::string line;
    while (true) {
        out << "exponent? ";
        if (!std::getline(in, line)) return false;
        if (parse_int_prefix(line, exponent) && exponent >= 0 && exponent <= MAX_EXPONENT) {
            return true;
        }
        out << "exponent must be integer, and exponent>=0 and exponent<=1000" << std::endl;
    }
}

// 将浮点数转换成较简洁的输出形式。
static std::string format_number(double value) {
    if (std::fabs(value) < EPS) value = 0.0;

    std::ostringstream oss;
    oss << std::setprecision(12) << std::defaultfloat << value;
    std::string s = oss.str();

    if (s == "-0") return "0";
    return s;
}

// 读取多项式，要求指数按降序输入。
bool Polynomial::read(std::istream &in, std::ostream &out) {
    terms_.clear();

    out << "Enter the coefficients and exponents for the polynomial, one pair per line." << std::endl;
    out << "Exponents(>=0) must be in descending order." << std::endl;
    out << "Enter a coefficient of 0 or an exponent of 0 to terminate." << std::endl;

    int previous_exponent = MAX_EXPONENT + 1;
    std::queue<Term> input_terms;

    while (true) {
        double coefficient = 0.0;
        if (!read_coefficient(in, out, coefficient)) return false;
        if (std::fabs(coefficient) < EPS) break;

        int exponent = 0;
        if (!read_exponent(in, out, exponent)) return false;
        if (exponent == 0) break;

        if (exponent >= previous_exponent) {
            out << "Bad exponent: Polynomial terminates without its last term." << std::endl;
            break;
        }

        input_terms.push(Term{coefficient, exponent});
        previous_exponent = exponent;
    }

    while (!input_terms.empty()) {
        add_term(input_terms.front().coefficient, input_terms.front().exponent);
        input_terms.pop();
    }
    normalize();
    return true;
}

// 输出当前多项式。
void Polynomial::print(std::ostream &out) const {
    if (terms_.empty()) {
        out << "0";
        return;
    }

    bool first = true;
    for (const Term &term : terms_) {
        if (std::fabs(term.coefficient) < EPS) continue;

        double abs_coef = std::fabs(term.coefficient);
        if (first) {
            if (term.coefficient < 0) out << "-";
        } else {
            out << (term.coefficient < 0 ? " - " : " + ");
        }

        out << format_number(abs_coef);
        if (term.exponent != 0) {
            out << " X^" << term.exponent;
        }
        first = false;
    }

    if (first) out << "0";
}

// 把两个多项式的和保存到当前对象。
void Polynomial::equals_sum(const Polynomial &p, const Polynomial &q) {
    *this = p + q;
}

// 把两个多项式的差保存到当前对象。
void Polynomial::equals_difference(const Polynomial &p, const Polynomial &q) {
    *this = p - q;
}

// 返回最高指数。
int Polynomial::degree() const {
    if (terms_.empty()) return -1;
    return terms_.front().exponent;
}

// 返回当前多项式与另一个多项式相加后的结果。
Polynomial Polynomial::added_with(const Polynomial &another) const {
    return *this + another;
}

// 用当前多项式乘以一个数字并返回结果。
Polynomial Polynomial::multiplied_by_scalar(double scalar) const {
    Polynomial result;
    if (std::fabs(scalar) < EPS || terms_.empty()) return result;

    for (const Term &term : terms_) {
        result.terms_.push_back(Term{term.coefficient * scalar, term.exponent});
    }
    result.normalize();
    return result;
}

// 用当前多项式乘以一个单项式并返回结果。
Polynomial Polynomial::multiplied_by_term(double coefficient, int exponent) const {
    Polynomial result;
    if (std::fabs(coefficient) < EPS || terms_.empty()) return result;

    for (const Term &term : terms_) {
        long long new_exponent = static_cast<long long>(term.exponent) + exponent;
        if (new_exponent > MAX_EXPONENT) {
            continue;
        }
        result.add_term(term.coefficient * coefficient, static_cast<int>(new_exponent));
    }
    result.normalize();
    return result;
}

// 按指数顺序插入新项，若指数相同则合并系数。
void Polynomial::add_term(double coefficient, int exponent) {
    if (std::fabs(coefficient) < EPS) return;

    for (auto it = terms_.begin(); it != terms_.end(); ++it) {
        if (it->exponent == exponent) {
            it->coefficient += coefficient;
            if (std::fabs(it->coefficient) < EPS) {
                terms_.erase(it);
            }
            return;
        }
        if (it->exponent < exponent) {
            terms_.insert(it, Term{coefficient, exponent});
            return;
        }
    }
    terms_.push_back(Term{coefficient, exponent});
}

// 清除系数接近 0 的项。
void Polynomial::normalize() {
    for (auto it = terms_.begin(); it != terms_.end();) {
        if (std::fabs(it->coefficient) < EPS) {
            it = terms_.erase(it);
        } else {
            ++it;
        }
    }
}

// 多项式加法运算符。
Polynomial operator+(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial result;
    auto i = lhs.terms_.begin();
    auto j = rhs.terms_.begin();

    while (i != lhs.terms_.end() && j != rhs.terms_.end()) {
        if (i->exponent > j->exponent) {
            result.terms_.push_back(*i++);
        } else if (i->exponent < j->exponent) {
            result.terms_.push_back(*j++);
        } else {
            double coefficient = i->coefficient + j->coefficient;
            if (std::fabs(coefficient) >= EPS) {
                result.terms_.push_back(Term{coefficient, i->exponent});
            }
            ++i;
            ++j;
        }
    }

    while (i != lhs.terms_.end()) result.terms_.push_back(*i++);
    while (j != rhs.terms_.end()) result.terms_.push_back(*j++);
    result.normalize();
    return result;
}

// 多项式减法运算符。
Polynomial operator-(const Polynomial &lhs, const Polynomial &rhs) {
    Polynomial result;
    auto i = lhs.terms_.begin();
    auto j = rhs.terms_.begin();

    while (i != lhs.terms_.end() && j != rhs.terms_.end()) {
        if (i->exponent > j->exponent) {
            result.terms_.push_back(*i++);
        } else if (i->exponent < j->exponent) {
            result.terms_.push_back(Term{-j->coefficient, j->exponent});
            ++j;
        } else {
            double coefficient = i->coefficient - j->coefficient;
            if (std::fabs(coefficient) >= EPS) {
                result.terms_.push_back(Term{coefficient, i->exponent});
            }
            ++i;
            ++j;
        }
    }

    while (i != lhs.terms_.end()) result.terms_.push_back(*i++);
    while (j != rhs.terms_.end()) {
        result.terms_.push_back(Term{-j->coefficient, j->exponent});
        ++j;
    }
    result.normalize();
    return result;
}

// 多项式数乘运算符。
Polynomial operator*(const Polynomial &p, double scalar) {
    return p.multiplied_by_scalar(scalar);
}

// 多项式数乘运算符，支持数字写在左边。
Polynomial operator*(double scalar, const Polynomial &p) {
    return p.multiplied_by_scalar(scalar);
}

// 使用输出运算符输出多项式。
std::ostream &operator<<(std::ostream &out, const Polynomial &p) {
    p.print(out);
    return out;
}
