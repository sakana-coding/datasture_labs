#include <iostream>
#include <string>
#include "Mystack.h"

int main()
{
    // 交互标题
    std::cout << "Bracket Matching Program." << std::endl;
    std::cout << "Input a string line:" << std::endl;

    // 读取用户输入的整行字符串
    std::string inputStr;
    std::getline(std::cin, inputStr);

    Mystack<char> bracketStack;
    bool hasMismatch = false;

    // 遍历每个字符，执行括号匹配逻辑
    for (char ch : inputStr)
    {
        // 左括号：入栈
        if (ch == '(' || ch == '{' || ch == '[')
        {

            bracketStack.push(ch);
        }
        // 右括号：出栈匹配
        else if (ch == ')' || ch == '}' || ch == ']')
        {
            // 栈空：右括号无匹配的左括号
            if (bracketStack.empty())
            {
                std::cout << "2. Unmatched closing bracket(s) detected." << std::endl;
                hasMismatch = true;
                break;
            }

            // 弹出栈顶左括号，检查匹配性
            char topBracket = bracketStack.top();
            bracketStack.pop();

            bool isMatch = false;
            if (ch == ')' && topBracket == '(')
                isMatch = true;
            else if (ch == '}' && topBracket == '{')
                isMatch = true;
            else if (ch == ']' && topBracket == '[')
                isMatch = true;

            // 括号类型不匹配
            if (!isMatch)
            {
                std::cout << "Bad match " << topBracket << ch << std::endl;
                hasMismatch = true;
            }
        }
        // 非括号字符：直接忽略，不影响匹配结果
        else
        {
            continue;
        }
    }

    // 最终结果输出
    if (!hasMismatch)
    {
        if (bracketStack.empty())
        {
            std::cout << "0. Ok, matched!" << std::endl;
        }
        else
        {
            std::cout << "1. Unmatched opening bracket(s) detected." << std::endl;
        }
    }
    else
    {
        // 存在不匹配时，额外检查是否有剩余未匹配的左括号
        if (!bracketStack.empty())
        {
            std::cout << "1. Unmatched opening bracket(s) detected." << std::endl;
        }
    }

    return 0;
}