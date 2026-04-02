#include "Myqueue.h"
#include <iostream>

int main() {
    MyQueue q1;
    q1.append('A');
    q1.append('B');

    MyQueue q2 = q1;
    q1.serve();

    std::cout << q1.toString() << "\n";
    std::cout << q2.toString() << "\n";

    MyQueue q3;
    q3.append('X');
    q3 = q2;
    q2.serve();

    std::cout << q2.toString() << "\n";
    std::cout << q3.toString() << "\n";
    return 0;
}
