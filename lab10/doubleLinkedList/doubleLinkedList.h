#pragma once
#include <iostream>
using namespace std;

enum Error_code { underflow, overflow, range_error, success };

template <class List_entry>
struct Node {
    List_entry data;
    Node<List_entry> *next;
    Node<List_entry> *prev;
    Node(List_entry d = List_entry(), Node *p = nullptr, Node *n = nullptr)
        : data(d), prev(p), next(n) {}
};

template <class List_entry>
class List {
public:
    ~List();
    List();
    List(const List<List_entry> &copy);
    List<List_entry>& operator=(const List<List_entry> &copy);
    int size() const;
    bool full() const;
    bool empty() const;
    void clear();
    void traverse(void (*visit)(List_entry &));
    Error_code retrieve(int position, List_entry &x) const;
    Error_code replace(int position, const List_entry &x);
    Error_code remove(int position, List_entry &x);
    Error_code insert(int position, const List_entry &x);

protected:
    int count;
    Node<List_entry> *head;
    Node<List_entry> *set_position(int position) const;
};

template <class List_entry>
List<List_entry>::List() : count(0), head(nullptr) {}

template <class List_entry>
List<List_entry>::~List() { clear(); }

template <class List_entry>
List<List_entry>::List(const List<List_entry>& copy) : count(0), head(nullptr) {
    *this = copy;
}

template <class List_entry>
List<List_entry>& List<List_entry>::operator=(const List<List_entry>& copy) {
    if (this == &copy) return *this;
    clear();
    Node<List_entry>* p = copy.head;
    for (int i = 0; i < copy.count; i++, p = p->next)
        insert(i, p->data);
    return *this;
}

template <class List_entry>
int List<List_entry>::size() const { return count; }

template <class List_entry>
bool List<List_entry>::full() const { return false; }

template <class List_entry>
bool List<List_entry>::empty() const { return count == 0; }

template <class List_entry>
void List<List_entry>::clear() {
    while (head) {
        Node<List_entry>* tmp = head;
        head = head->next;
        delete tmp;
    }
    count = 0;
}

template <class List_entry>
Node<List_entry>* List<List_entry>::set_position(int position) const {
    Node<List_entry>* p = head;
    for (int i = 0; i < position; i++) p = p->next;
    return p;
}

template <class List_entry>
void List<List_entry>::traverse(void (*visit)(List_entry&)) {
    for (Node<List_entry>* p = head; p; p = p->next)
        (*visit)(p->data);
}

template <class List_entry>
Error_code List<List_entry>::retrieve(int position, List_entry& x) const {
    if (position < 0 || position >= count) return Error_code::range_error;
    x = set_position(position)->data;
    return success;
}

template <class List_entry>
Error_code List<List_entry>::replace(int position, const List_entry& x) {
    if (position < 0 || position >= count) return Error_code::range_error;
    set_position(position)->data = x;
    return success;
}

template <class List_entry>
Error_code List<List_entry>::remove(int position, List_entry& x) {
    if (count == 0) return underflow;
    if (position < 0 || position >= count) return Error_code::range_error;
    Node<List_entry>* p = set_position(position);
    x = p->data;
    if (p->prev) p->prev->next = p->next;
    else head = p->next;
    if (p->next) p->next->prev = p->prev;
    delete p;
    count--;
    return success;
}

template <class List_entry>
Error_code List<List_entry>::insert(int position, const List_entry& x) {
    if (position < 0 || position > count) return Error_code::range_error;
    Node<List_entry>* prev_node = (position > 0) ? set_position(position - 1) : nullptr;
    Node<List_entry>* next_node = prev_node ? prev_node->next : head;
    Node<List_entry>* p = new Node<List_entry>(x, prev_node, next_node);
    if (prev_node) prev_node->next = p;
    else head = p;
    if (next_node) next_node->prev = p;
    count++;
    return success;
}
