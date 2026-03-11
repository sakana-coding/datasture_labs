#include "Mystack.h"
#include <vector>
#include <stdexcept>
bool Mystack::empty() const
{
    return count == 0;
}
double Mystack::top() const
{
    if (empty())
    {
        throw std::out_of_range("stack is empty");
    }
    return vec[count - 1];
}
void Mystack::pop()
{
    if (!empty())
    {
        vec.pop_back();
        count--;
    }
}
int Mystack::size() const
{
    return count;
}
void Mystack::push(double num)
{
    vec.push_back(num);
    count++;
}
