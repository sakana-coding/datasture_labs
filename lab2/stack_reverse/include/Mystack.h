#ifndef MYSTACK_H
#define MYSTACK_H
#include <vector>
class Mystack
{
public:
    //初始化栈，使用列表初始化
    Mystack() : count(0), vec() {
                };
    double top() const;
    void pop();
    void push(double);
    bool empty() const;

private:
    int count;
    const int MaxSize = 10;
    std::vector<double> vec;
};

#endif