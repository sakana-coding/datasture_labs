#ifndef MYSTACK_H
#define MYSTACK_H
#include <vector>
template<typename T>
class Mystack
{
public:
    // 初始化栈，使用列表初始化
    Mystack() : count(0), vec() {
                };
    T top() const;
    void pop();
    void push(T);
    bool empty() const;
    // 加一个size方法，可以间接访问private的count
    int size() const;

private:
    int count;
    std::vector<T> vec;
};
#include "Mystack.tpp"
#endif