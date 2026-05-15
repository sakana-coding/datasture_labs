#include <iostream>
#include "doubleLinkedList.h"

using namespace std;

void print(int &x) { cout << x <<"\n"; }
void update(int &x) { x *= 2; }

int main() {
    List<int> mylist;
    int i;
    for (i = 0; i < 5; i++) mylist.insert(i, i);
    cout << "Your list have " << mylist.size() << " elements:" << endl;
    mylist.traverse(print);
    cout << endl;

    mylist.remove(1, i);
    cout << "After remove(1):" << endl;
    mylist.traverse(print);
    cout << endl;

    mylist.remove(0, i);
    cout << "After remove(0):" << endl;
    mylist.traverse(print);
    cout << endl;

    List<int> mylist2(mylist);
    cout << "After mylist2(mylist):" << endl;
    mylist2.traverse(print);
    cout << endl;

    cout << "After update:" << endl;
    mylist.traverse(update);
    mylist.traverse(print);
    cout << endl;

    List<int> mylist3;
    mylist3 = mylist;
    cout << "After mylist3=mylist:" << endl;
    mylist3.traverse(print);
    cout << endl;

    return 0;
}
