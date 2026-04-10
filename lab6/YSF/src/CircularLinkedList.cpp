#include "CircularLinkedList.h"

// 初始为空表。
CircularLinkedList::CircularLinkedList() : tail_(0), current_(0), size_(0) {}

CircularLinkedList::~CircularLinkedList() {
    clear();
}

bool CircularLinkedList::append(int value) {
    Node *newNode = new Node(value);

    // 空表时，新结点既是头结点也是尾结点，next 指向自己。
    if (tail_ == 0) {
        tail_ = newNode;
        tail_->next = tail_;
        current_ = tail_;
    } else {
        // 非空时采用尾插法，并保持循环结构。
        newNode->next = tail_->next;
        tail_->next = newNode;
        tail_ = newNode;
    }

    ++size_;
    return true;
}

bool CircularLinkedList::createSequentialList(int n) {
    // 重新创建前先清空旧数据。
    clear();

    if (n <= 0) {
        return false;
    }

    // 顺序插入 1~n。
    for (int i = 1; i <= n; ++i) {
        append(i);
    }

    // 约瑟夫环初始从 1 号开始报数，因此 current_ 指向头结点。
    current_ = tail_->next;
    return true;
}

bool CircularLinkedList::eliminateByStep(int step, int &removedValue) {
    if (isEmpty() || step <= 0) {
        return false;
    }

    Node *previous = tail_;
    Node *target = current_;

    // 从 current_ 开始报数，走到第 step 个结点。
    for (int i = 1; i < step; ++i) {
        previous = target;
        target = target->next;
    }

    removedValue = target->data;

    // 只剩一个结点时，删除后链表变为空表。
    if (size_ == 1) {
        delete target;
        tail_ = 0;
        current_ = 0;
        size_ = 0;
        return true;
    }

    // 将 target 从循环链表中摘下。
    previous->next = target->next;

    // 若删除的是尾结点，需要修正尾指针。
    if (target == tail_) {
        tail_ = previous;
    }

    // 下一轮从被删结点的下一个结点开始报数。
    current_ = target->next;
    delete target;
    --size_;
    return true;
}

bool CircularLinkedList::getCurrentValue(int &value) const {
    if (isEmpty()) {
        return false;
    }

    value = current_->data;
    return true;
}

bool CircularLinkedList::isEmpty() const {
    return size_ == 0;
}

int CircularLinkedList::size() const {
    return size_;
}

void CircularLinkedList::clear() {
    if (tail_ == 0) {
        return;
    }

    // 先断开环，再按普通单链表方式逐个释放。
    Node *head = tail_->next;
    tail_->next = 0;

    while (head != 0) {
        Node *nextNode = head->next;
        delete head;
        head = nextNode;
    }

    tail_ = 0;
    current_ = 0;
    size_ = 0;
}
