#include "Mystack.h"
#include <vector>
#include <stdexcept>
template<typename T>
bool Mystack<T>::empty() const
{
    return count == 0;
}
template <typename T>
T Mystack<T>::top() const
{
    if (empty())
    {
        throw std::out_of_range("stack is empty");
    }
    return vec[count - 1];
}
template <typename T>
void Mystack<T>::pop()
{
    if (!empty())
    {
        vec.pop_back();
        count--;
    }
}
template <typename T>
int Mystack<T>::size() const
{
    return count;
}
template <typename T>
void Mystack<T>::push(T num)
{
    vec.push_back(num);
    count++;
}
