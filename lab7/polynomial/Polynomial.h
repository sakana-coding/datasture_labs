#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <iosfwd>
#include <list>

struct Term {
    double coefficient{};
    int exponent{};
};

class Polynomial {
public:
    Polynomial() = default;
    Polynomial(const Polynomial &original) = default;
    Polynomial &operator=(const Polynomial &original) = default;

    // 从输入流读取一个多项式，遇到系数 0 或指数 0 时结束。
    bool read(std::istream &in, std::ostream &out);

    // 按常见数学形式输出多项式。
    void print(std::ostream &out) const;

    // 令当前多项式等于 p 和 q 的和。
    void equals_sum(const Polynomial &p, const Polynomial &q);

    // 令当前多项式等于 p 和 q 的差。
    void equals_difference(const Polynomial &p, const Polynomial &q);

    // 返回多项式最高次数，零多项式返回 -1。
    int degree() const;

    // 返回当前多项式加上 another 后的新多项式。
    Polynomial added_with(const Polynomial &another) const;

    // 返回当前多项式乘以一个普通数字后的结果。
    Polynomial multiplied_by_scalar(double scalar) const;

    // 返回当前多项式乘以一个单项式后的结果。
    Polynomial multiplied_by_term(double coefficient, int exponent) const;

    bool empty() const { return terms_.empty(); }

    friend Polynomial operator+(const Polynomial &lhs, const Polynomial &rhs);
    friend Polynomial operator-(const Polynomial &lhs, const Polynomial &rhs);
    friend Polynomial operator*(const Polynomial &p, double scalar);
    friend Polynomial operator*(double scalar, const Polynomial &p);

private:
    // 链表中的项始终按指数从大到小排列。
    std::list<Term> terms_;

    // 插入或合并一个多项式项。
    void add_term(double coefficient, int exponent);

    // 删除系数为 0 的项。
    void normalize();
};

std::ostream &operator<<(std::ostream &out, const Polynomial &p);

#endif
