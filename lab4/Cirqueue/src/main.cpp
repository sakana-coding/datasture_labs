#include "Myqueue.h"

#include <cctype>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace {

constexpr int kMinN = 1;
constexpr int kMaxN = 10;

std::string trim(const std::string& text) {
    std::size_t left = 0;
    while (left < text.size() && std::isspace(static_cast<unsigned char>(text[left]))) {
        ++left;
    }

    std::size_t right = text.size();
    while (right > left && std::isspace(static_cast<unsigned char>(text[right - 1]))) {
        --right;
    }

    return text.substr(left, right - left);
}

bool parseIntLine(const std::string& line, int& value) {
    std::string cleaned = trim(line);
    if (cleaned.empty()) {
        return false;
    }

    std::istringstream input(cleaned);
    long long candidate = 0;
    char extra = '\0';
    if (!(input >> candidate) || (input >> extra)) {
        return false;
    }

    if (candidate < std::numeric_limits<int>::min() || candidate > std::numeric_limits<int>::max()) {
        return false;
    }

    value = static_cast<int>(candidate);
    return true;
}

bool parseDoubleToken(const std::string& token, double& value) {
    try {
        std::size_t pos = 0;
        double candidate = std::stod(token, &pos);
        if (pos != token.size() || !std::isfinite(candidate)) {
            return false;
        }
        value = candidate;
        return true;
    } catch (...) {
        return false;
    }
}

int readValidN() {
    std::string line;
    int n = 0;

    while (std::getline(std::cin, line)) {
        if (parseIntLine(line, n) && n >= kMinN && n <= kMaxN) {
            return n;
        }
        std::cout << "n must be integer, and n>=1 and n<=10\n";
    }

    return 0;
}

bool readDecimals(MyQueue& queue, int n) {
    std::string token;
    int count = 0;

    while (count < n && (std::cin >> token)) {
        double value = 0.0;
        if (!parseDoubleToken(token, value)) {
            std::cout << "Please input a double value!\n";
            continue;
        }

        queue.append(value);
        ++count;
    }

    return count == n;
}

void printQueueInOrder(MyQueue& queue) {
    bool first = true;
    while (!queue.empty()) {
        MyQueue::ValueType value = 0.0;
        queue.retrieve(value);
        if (!first) {
            std::cout << ' ';
        }
        std::cout << value;
        queue.serve();
        first = false;
    }
    std::cout << '\n';
}

}  // namespace

int main() {
    std::cout << "Type in an integer n followed by n decimal numbers.\n";
    std::cout << "The numbers will be printed in original order.\n";

    const int n = readValidN();
    if (n == 0) {
        return 0;
    }

    std::cout << "Please input " << n << " decimal numbers:\n";

    MyQueue queue;
    if (!readDecimals(queue, n)) {
        return 0;
    }

    std::cout << '\n';
    printQueueInOrder(queue);
    return 0;
}
