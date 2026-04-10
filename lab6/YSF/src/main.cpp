#include <iostream>
#include <limits>
#include "CircularLinkedList.h"

// n 和 m 的范围均为 [1, 1000]。
const int kMinValue = 1;
const int kMaxValue = 1000;
// 输入缓冲区长度，用于接收一整行文本并自行校验。
const int kInputBufferSize = 256;

// 若 getline 因输入过长而失败，则清除错误状态并丢弃该行剩余内容。
void clearRemainingLineIfNeeded() {
    if (!std::cin.fail()) {
        return;
    }

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// 判断一行输入是否为合法的 [minValue, maxValue] 正数。
// 允许前后空格；若输入小数，则只取整数部分
// 不允许字母、多个小数点等非法字符混入数字中。
bool parsePositiveIntegerInRange(const char *text, int &value, int minValue, int maxValue) {
    if (text == 0) {
        return false;
    }

    int index = 0;
    while (text[index] == ' ' || text[index] == '\t') {
        ++index;
    }

    if (text[index] == '\0') {
        return false;
    }

    bool negative = false;
    if (text[index] == '+' || text[index] == '-') {
        negative = (text[index] == '-');
        ++index;
    }

    if (text[index] == '\0') {
        return false;
    }

    long long result = 0;
    bool hasDigit = false;
    while (text[index] >= '0' && text[index] <= '9') {
        hasDigit = true;
        result = result * 10 + (text[index] - '0');
        if (result > maxValue) {
            return false;
        }
        ++index;
    }

    // 支持小数输入，但只保留整数部分。例如输入 3.6，实际按 3 处理。
    if (text[index] == '.') {
        ++index;
        while (text[index] >= '0' && text[index] <= '9') {
            ++index;
        }
    }

    // 数字后面只能跟空格或字符串结束，不能出现 abc、第二个小数点等非法内容。
    while (text[index] == ' ' || text[index] == '\t') {
        ++index;
    }

    if (text[index] != '\0') {
        return false;
    }

    if (negative || !hasDigit) {
        return false;
    }

    if (result < minValue || result > maxValue) {
        return false;
    }

    value = static_cast<int>(result);
    return true;
}

// 循环读取一行，直到用户输入一个合法数值。
int readIntegerWithValidation(const char *prompt, int minValue, int maxValue) {
    char buffer[kInputBufferSize];
    int value = 0;

    while (true) {
        std::cout << prompt;
        std::cin.getline(buffer, kInputBufferSize);

        if (std::cin.fail()) {
            clearRemainingLineIfNeeded();
            continue;
        }

        if (parsePositiveIntegerInRange(buffer, value, minValue, maxValue)) {
            return value;
        }
    }
}

int main() {
    // 输出题目提示信息。
    std::cout << "This is a YSF(n,m) problem" << std::endl;

    // 读取人数 n 和报数上限 m，并进行鲁棒性校验。
    int n = readIntegerWithValidation(
        "Please input the integer n(n>=1 and n<=1000).",
        kMinValue,
        kMaxValue);

    int m = readIntegerWithValidation(
        "Please input the integer m(m>=1 and m<=1000).",
        kMinValue,
        kMaxValue);

    // 建立 1~n 的循环链表。
    CircularLinkedList circle;
    circle.createSequentialList(n);

    std::cout << std::endl;

    // 按约瑟夫环规则不断删除，直到只剩最后一个结点。
    int removedValue = 0;
    while (circle.size() > 1) {
        circle.eliminateByStep(m, removedValue);
        std::cout << "Get rid of:" << removedValue << std::endl;
    }

    // 输出最后留下的元素。
    int leftValue = 0;
    if (circle.getCurrentValue(leftValue)) {
        std::cout << "The left element is:" << leftValue << std::endl;
    }

    // 等待用户输入后退出，便于在控制台查看结果。
    std::cout << "Press any key to quit.";
    std::cout.flush();
    std::cin.get();

    return 0;
}
