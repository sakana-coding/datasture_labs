#include "Mystack.h"
#include <vector>
#include <stdexcept>
// Mystack类函数实现
bool Mystack::empty() const
{
    return count == 0;
}
// 输出栈顶元素
double Mystack::top() const
{
    if (empty())
    {
        throw std::out_of_range("stack is empty");
    }
    return vec[count - 1];
}
// 推出栈顶元素
void Mystack::pop()
{
    if (!empty())
    {
        vec.pop_back();
        count--;
    }
}
// 压栈
void Mystack::push(double num)
{
    if (count == 10)
        return;
    vec.push_back(num);
    count++;
}
