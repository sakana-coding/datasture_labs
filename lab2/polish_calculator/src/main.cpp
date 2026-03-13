#include <iostream>
#include <limits>
#include <cmath>
#include <cstdlib>
#include "Mystack.h"

using namespace std;

// 函数声明 , 注意这里要使用引用才能对实参进行修改
void printHelpMenu();
void Push(Mystack &stack);
void PrintTop(Mystack &stack);
void Add(Mystack &stack);
void Subtract(Mystack &stack);
void Multiply(Mystack &stack);
void Divide(Mystack &stack);
void Power(Mystack &stack);
void Quit();

int main()
{
    Mystack calcStack;
    char command;
    // 打印prompt头
    cout << "This is a reverse Polish Calculator." << endl;
    cout << "Please enter a valid command:" << endl;
    printHelpMenu();

    while (true)
    {
        cout << "Select command and press <Enter>:";

        if (!(cin >> command))
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Please enter a valid command:" << endl;
            printHelpMenu();
            continue;
        }
        // 这一句很关键，需要把读入命令后的所有都删掉，防止影响到下一次读入
        cin.ignore(INT_MAX, '\n');
        // 根据选项来处理
        if (command == '?')
        {
            Push(calcStack);
        }
        else if (command == '=')
        {
            PrintTop(calcStack);
        }
        else if (command == '+')
        {
            Add(calcStack);
        }
        else if (command == '-')
        {
            Subtract(calcStack);
        }
        else if (command == '*')
        {
            Multiply(calcStack);
        }
        else if (command == '/')
        {
            Divide(calcStack);
        }
        else if (command == '^')
        {
            Power(calcStack);
        }
        else if (command == 'Q' || command == 'q')
        {
            Quit();
        }
        else
        {
            cout << "Please enter a valid command:" << endl;
            printHelpMenu();
        }
    }

    return 0;
}

// 函数实现（包括鲁棒性）
// 打印菜单
void printHelpMenu()
{
    cout << "[?]push to stack   [=]print top\n";
    cout << "[+] [-] [*] [/] [^]   are arithmetic operations\n";
    cout << "[Q]uit.\n";
}
// 推入
void Push(Mystack &stack)
{
    double num;
    while (true)
    {
        cout << "Enter a real number: ";
        if (!(cin >> num))
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            cout << "Must be a real number!" << endl;
        }
        else
        {
            stack.push(num);
            cin.ignore(INT_MAX, '\n');
            break;
        }
    }
}
// 打印栈顶元素
void PrintTop(Mystack &stack)
{
    if (stack.empty())
    {
        cout << "Error: Stack is empty, no top element to print!" << endl;
    }
    else
    {
        cout << stack.top() << endl;
    }
}
// 加法
void Add(Mystack &stack)
{
    if (stack.size() < 2)
    {
        cout << "Error: Not enough operands! Need at least 2 numbers in stack." << endl;
        return;
    }
    double right = stack.top();
    stack.pop();
    double left = stack.top();
    stack.pop();
    stack.push(left + right);
}
// 减法
void Subtract(Mystack &stack)
{
    if (stack.size() < 2)
    {
        cout << "Error: Not enough operands! Need at least 2 numbers in stack." << endl;
        return;
    }
    double right = stack.top();
    stack.pop();
    double left = stack.top();
    stack.pop();
    stack.push(left - right);
}
// 乘法
void Multiply(Mystack &stack)
{
    if (stack.size() < 2)
    {
        cout << "Error: Not enough operands! Need at least 2 numbers in stack." << endl;
        return;
    }
    double right = stack.top();
    stack.pop();
    double left = stack.top();
    stack.pop();
    stack.push(left * right);
}
// 除法
void Divide(Mystack &stack)
{
    if (stack.size() < 2)
    {
        cout << "Error: Not enough operands! Need at least 2 numbers in stack." << endl;
        return;
    }
    double right = stack.top();
    stack.pop();
    double left = stack.top();
    stack.pop();
    // 除数为0的逻辑处理
    if (right == 0)
    {
        cout << "Error: Division by zero!Two numbers that poped are pushed into stack again" << endl;
        stack.push(left);
        stack.push(right);
        return;
    }
    stack.push(left / right);
}
// 幂运算
void Power(Mystack &stack)
{
    if (stack.size() < 2)
    {
        cout << "Error: Not enough operands! Need at least 2 numbers in stack." << endl;
        return;
    }
    double right = stack.top();
    stack.pop();
    double left = stack.top();
    stack.pop();
    // 处理0^0
    if (right == 0 && left == 0)
    {
        stack.push(1);
        return;
    }
    stack.push(pow(left, right));
}

void Quit()
{
    cout << "Calculation finished." << endl;
    exit(0);
}