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
        //这一句很关键，需要把读入命令后的所有都删掉，防止影响到下一次读入
        cin.ignore(INT_MAX, '\n');

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

// 函数实现
void printHelpMenu()
{
    cout << "[?]push to stack   [=]print top\n" ;
    cout << "[+] [-] [*] [/] [^]   are arithmetic operations\n";
    cout << "[Q]uit.\n";
}

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

    if (right == 0)
    {
        cout << "Error: Division by zero!" << endl;
        stack.push(left);
        stack.push(right);
        return;
    }
    stack.push(left / right);
}

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
    stack.push(pow(left, right));
}

void Quit()
{
    cout << "Calculation finished." << endl;
    exit(0);
}