#include "Myqueue.h"
#include <iostream>
#include <utility>

int main() {
    MyQueue q1;
    q1.append('A');
    q1.append('B');

    MyQueue q2(std::move(q1));
    std::cout << q2.toString() << "\n";
    std::cout << q1.size() << "\n";

    MyQueue q3;
    q3.append('X');
    q3 = std::move(q2);
    std::cout << q3.toString() << "\n";
    std::cout << q2.size() << "\n";
    return 0;
}
