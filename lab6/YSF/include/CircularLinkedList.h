#ifndef CIRCULAR_LINKED_LIST_H
#define CIRCULAR_LINKED_LIST_H

class CircularLinkedList {
public:
    // 构造空循环链表。
    CircularLinkedList();
    // 析构时释放全部结点，避免内存泄漏。
    ~CircularLinkedList();

    // 创建 1~n 的循环链表。
    bool createSequentialList(int n);
    // 从 current_ 开始报数，删除第 step 个结点，并返回被删除的编号。
    bool eliminateByStep(int step, int &removedValue);
    // 获取当前报数起点对应的编号。
    bool getCurrentValue(int &value) const;
    // 判断链表是否为空。
    bool isEmpty() const;
    // 返回当前链表中的结点个数。
    int size() const;
    // 清空循环链表。
    void clear();

private:
    // 链表结点定义。
    struct Node {
        int data;
        Node *next;

        explicit Node(int value) : data(value), next(0) {}
    };

    Node *tail_;    // 尾指针，便于 O(1) 找到头结点。
    Node *current_; // 当前报数起点。
    int size_;      // 当前链表长度。

    // 在尾部插入新结点。
    bool append(int value);
};

#endif
