#include <iostream>
#include <limits>
#include "Mystack.h"

using namespace std;

int main()
{
    // 输入并校验十进制数N
    long long n;
    while (true)
    {
        cout << "输入要转化的十进制数N:\n";
        cin >> n;
        // 校验：非整数、超出范围[1, 2147483647]均报错
        if (cin.fail() || n < 1 || n > 2147483647)
        {
            cin.clear(); 
            // 忽略缓冲区所有错误内容，直到换行符
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "n must be integer, and n>=1 and n<=2147483647" << endl;
        }
        else
        {
            break;
        }
    }

    // 输入并校验目标进制r
    int r;
    while (true)
    {
        cout << "输入要转化的进制数r:\n";
        cin >> r;
        // 校验：非整数、超出范围[2,16]均报错
        if (cin.fail() || r < 2 || r > 16)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "r must be integer, and r>=2 and r<=16" << endl;
        }
        else
        {
            break;
        }
    }

    // 进制转换核心逻辑：除r取余，栈存储余数
    Mystack<int> stack;
    int num = static_cast<int>(n);
    while (num > 0)
    {
        int remainder = num % r;
        stack.push(remainder);
        num = num / r;
    }

    // 出栈输出，10-15转为A-F
    while (!stack.empty())
    {
        int val = stack.top();
        stack.pop();
        if (val < 10)
        {
            cout << val;
        }
        else
        {
            cout << static_cast<char>('A' + val - 10);
        }
        cout << " ";
    }
    cout << endl;

    return 0;
}