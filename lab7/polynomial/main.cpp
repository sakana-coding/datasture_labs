#include "Polynomial.h"

#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stack>
#include <string>

static constexpr int MAX_EXPONENT = 1000;

// 打印命令菜单。
static void print_menu() {
    std::cout << "Please enter a valid command:" << std::endl;
    std::cout << "[?] Read a Polynomial" << std::endl;
    std::cout << "[=] Return Top Polynomial" << std::endl;
    std::cout << "[+] Sum two Polynomial" << std::endl;
    std::cout << "[-] Difference two Polynomial" << std::endl;
    std::cout << "[*] Mult one Polynomial with a Term" << std::endl;
    std::cout << "[s] Mult one Polynomial with a Scalar" << std::endl;
    std::cout << "[q] Quit." << std::endl;
}

// 去除命令输入前后的空白字符。
static std::string trim_copy(const std::string &s) {
    std::size_t first = 0;
    while (first < s.size() && std::isspace(static_cast<unsigned char>(s[first]))) ++first;
    std::size_t last = s.size();
    while (last > first && std::isspace(static_cast<unsigned char>(s[last - 1]))) --last;
    return s.substr(first, last - first);
}

// 读取单项式系数时使用，能避免非法输入破坏后续读入。
static bool parse_double_prefix(const std::string &line, double &value) {
    std::string t = trim_copy(line);
    if (t.empty()) return false;
    char *end = nullptr;
    value = std::strtod(t.c_str(), &end);
    return end != t.c_str() && std::isfinite(value);
}

// 读取单项式指数时使用，能检查整数范围。
static bool parse_int_prefix(const std::string &line, int &value) {
    std::string t = trim_copy(line);
    if (t.empty()) return false;
    char *end = nullptr;
    long parsed = std::strtol(t.c_str(), &end, 10);
    if (end == t.c_str()) return false;
    if (parsed < std::numeric_limits<int>::min() || parsed > std::numeric_limits<int>::max()) return false;
    value = static_cast<int>(parsed);
    return true;
}

// 读取乘法命令中的单项式。
static bool read_term(double &coefficient, int &exponent) {
    std::string line;

    while (true) {
        std::cout << "coefficient? ";
        if (!std::getline(std::cin, line)) return false;
        if (parse_double_prefix(line, coefficient)) break;
        std::cout << "coefficient must be double!" << std::endl;
    }

    while (true) {
        std::cout << "exponent? ";
        if (!std::getline(std::cin, line)) return false;
        if (parse_int_prefix(line, exponent) && exponent >= 0 && exponent <= MAX_EXPONENT) break;
        std::cout << "exponent must be integer, and exponent>=0 and exponent<=1000" << std::endl;
    }

    return true;
}

// 读取数乘命令中的普通数字。
static bool read_scalar(double &scalar) {
    std::string line;

    while (true) {
        std::cout << "scalar? ";
        if (!std::getline(std::cin, line)) return false;
        if (parse_double_prefix(line, scalar)) return true;
        std::cout << "scalar must be double!" << std::endl;
    }
}

// 检查栈中是否有足够多项式参与运算。
static bool require_stack_size(const std::stack<Polynomial> &polynomials, std::size_t required) {
    if (polynomials.size() >= required) return true;
    if (required == 1) {
        std::cout << "Stack is empty. Please read a Polynomial first." << std::endl;
    } else {
        std::cout << "There are fewer than " << required << " Polynomials in the stack." << std::endl;
    }
    return false;
}

// 输出新结果入栈后的提示。
static void announce_push(const Polynomial &p) {
    std::cout << "The following Polynomial has been pushed into the stack:" << std::endl;
    std::cout << p << std::endl;
}

int main() {
    // 使用栈保存输入的多项式，所有运算都围绕栈顶元素进行。
    std::stack<Polynomial> polynomials;

    std::cout << "This is a polynomials program." << std::endl;
    print_menu();

    std::string line;
    while (true) {
        std::cout << "Select command and press <Enter>:";
        if (!std::getline(std::cin, line)) {
            std::cout << std::endl;
            break;
        }

        // 只取输入的第一个非空白字符作为命令，增强容错性。
        std::string trimmed = trim_copy(line);
        if (trimmed.empty()) {
            print_menu();
            continue;
        }

        char command = trimmed[0];
        switch (command) {
            case '?': {
                // 读入一个多项式并压入栈中。
                Polynomial p;
                if (!p.read(std::cin, std::cout)) return 0;
                polynomials.push(p);
                announce_push(polynomials.top());
                break;
            }
            case '=': {
                // 查看栈顶多项式但不弹出。
                if (require_stack_size(polynomials, 1)) {
                    std::cout << "The top Polynomial is:" << std::endl;
                    std::cout << polynomials.top() << std::endl;
                }
                break;
            }
            case '+': {
                // 弹出两个多项式，相加后把结果压回栈中。
                if (!require_stack_size(polynomials, 2)) break;
                Polynomial right = polynomials.top();
                polynomials.pop();
                Polynomial left = polynomials.top();
                polynomials.pop();
                Polynomial result = left.added_with(right);
                polynomials.push(result);
                announce_push(polynomials.top());
                break;
            }
            case '-': {
                // 弹出两个多项式，计算 left - right 后压回栈中。
                if (!require_stack_size(polynomials, 2)) break;
                Polynomial right = polynomials.top();
                polynomials.pop();
                Polynomial left = polynomials.top();
                polynomials.pop();
                Polynomial result;
                result.equals_difference(left, right);
                polynomials.push(result);
                announce_push(polynomials.top());
                break;
            }
            case '*': {
                // 读取一个单项式，令栈顶多项式乘以该单项式。
                if (!require_stack_size(polynomials, 1)) break;
                double coefficient = 0.0;
                int exponent = 0;
                if (!read_term(coefficient, exponent)) return 0;

                Polynomial operand = polynomials.top();
                polynomials.pop();
                Polynomial result = operand.multiplied_by_term(coefficient, exponent);
                polynomials.push(result);
                announce_push(polynomials.top());
                break;
            }
            case 's':
            case 'S': {
                // 读取一个普通数字，令栈顶多项式乘以该数字。
                if (!require_stack_size(polynomials, 1)) break;
                double scalar = 0.0;
                if (!read_scalar(scalar)) return 0;

                Polynomial operand = polynomials.top();
                polynomials.pop();
                Polynomial result = operand * scalar;
                polynomials.push(result);
                announce_push(polynomials.top());
                break;
            }
            case 'q':
            case 'Q':
                std::cout << "Quit." << std::endl;
                return 0;
            default:
                // 无效命令重新显示菜单。
                print_menu();
                break;
        }
    }

    return 0;
}
