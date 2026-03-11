#include <iostream>
#include <limits>
#include "Mystack.h"

using namespace std;

int getValidN()
{
    int n;
    bool flag = true;
    while (true)
    {
        if (flag)
        {
            cout << "Type in an integer n followed by n decimal numbers.\n"
                 << "The numbers will be printed in reverse order.\n";
            flag = false;
        }
        if (!(cin >> n))
        {
            cin.clear(); // 重置输入流错误状态
            // 清空缓冲区所有无效字符
            cin.ignore(INT_MAX, '\n');
            cout << "n must be integer, and n>=1 and n<=2147483647\n";
            continue;
        }
        // 校验n的范围
        if (n < 1 || n > 2147483647)
        {
            cout << "n must be integer, and n>=1 and n<=2147483647\n";
            continue;
        }
        // 输入合法，清空缓冲区（避免后续读取受影响）
        cin.ignore(INT_MAX, '\n');
        return n;
    }
}

// 获取合法的double类型实数
double getValidDouble(int index, int total)
{
    double num;
    while (true)
    {
        cout << "Please input " << total << " decimal numbers (" << index << "/" << total << "): ";
        // 校验是否为合法double
        if (!(cin >> num))
        {
            cin.clear(); // 重置错误状态
            cin.ignore(INT_MAX, '\n');
            cout << "Please input a double value!\n";
            continue;
        }
        return num;
    }
}

int main()
{
    Mystack stack;       // 创建自定义栈对象
    int n = getValidN(); // 获取合法的n

    // 输入n个实数并压栈
    for (int i = 1; i <= n; i++)
    {
        double num = getValidDouble(i, n);
        stack.push(num);
    }

    // 弹出栈中元素（逆序输出）
    cout << "Reverse order: ";
    while (!stack.empty())
    {
        cout << stack.top() << " ";
        stack.pop();
    }
    cout << endl;

    return 0;
}