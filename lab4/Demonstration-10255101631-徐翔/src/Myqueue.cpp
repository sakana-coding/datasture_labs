#include "Myqueue.h"

#include <sstream>

namespace {
// 队列底层数组的初始容量。
constexpr std::size_t kInitialCapacity = 10;
}

// 构造空队列：申请初始数组空间，并将队头、队尾、元素个数置为 0。
MyQueue::MyQueue()
    : data_(new char[kInitialCapacity]), capacity_(kInitialCapacity), front_(0), rear_(0), size_(0) {}

// 拷贝构造函数：申请新的数组空间，并复制另一个队列中的元素和状态。
MyQueue::MyQueue(const MyQueue& other)
    : data_(new char[other.capacity_]),
      capacity_(other.capacity_),
      front_(other.front_),
      rear_(other.rear_),
      size_(other.size_) {
    for (std::size_t i = 0; i < capacity_; ++i) {
        data_[i] = other.data_[i];
    }
}

// 赋值运算符：先判断是否自赋值，再释放旧空间并深拷贝新内容。
MyQueue& MyQueue::operator=(const MyQueue& other) {
    if (this == &other) {
        return *this;
    }

    char* newData = new char[other.capacity_];
    for (std::size_t i = 0; i < other.capacity_; ++i) {
        newData[i] = other.data_[i];
    }
    //删除旧空间防止内存泄漏
    delete[] data_;
    data_ = newData;
    capacity_ = other.capacity_;
    front_ = other.front_;
    rear_ = other.rear_;
    size_ = other.size_;

    return *this;
}

// 移动构造函数：直接接管另一个对象的底层数组和状态，再将源对象置空。
MyQueue::MyQueue(MyQueue&& other) noexcept
    : data_(other.data_),
      capacity_(other.capacity_),
      front_(other.front_),
      rear_(other.rear_),
      size_(other.size_) {
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.front_ = 0;
    other.rear_ = 0;
    other.size_ = 0;
}

// 移动赋值运算符：先释放当前对象原有资源，再接管源对象资源，最后将源对象置空。
MyQueue& MyQueue::operator=(MyQueue&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    delete[] data_;

    data_ = other.data_;
    capacity_ = other.capacity_;
    front_ = other.front_;
    rear_ = other.rear_;
    size_ = other.size_;

    other.data_ = nullptr;
    other.capacity_ = 0;
    other.front_ = 0;
    other.rear_ = 0;
    other.size_ = 0;

    return *this;
}

// 析构时释放动态申请的数组空间，防止内存泄漏。
MyQueue::~MyQueue() {
    delete[] data_;
}

// 当数组已满时，将容量扩大为原来的 2 倍，并按逻辑顺序重新拷贝元素。
void MyQueue::expand() {
    const std::size_t newCapacity = capacity_ * 2;
    char* newData = new char[newCapacity];

    // 按照队列从队头到队尾的顺序，将旧数组中的元素复制到新数组。
    for (std::size_t i = 0; i < size_; ++i) {
        newData[i] = data_[(front_ + i) % capacity_];
    }

    // 释放旧数组，并更新容量和队头队尾位置。
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
    front_ = 0;
    rear_ = size_;
}

// 在队尾插入一个字符；如果数组满了，先扩容再插入。
void MyQueue::append(char value) {
    if (size_ == capacity_) {
        expand();
    }

    data_[rear_] = value;
    rear_ = (rear_ + 1) % capacity_;
    ++size_;
}

// 删除队头元素；若队列为空则删除失败并返回 false。
bool MyQueue::serve() {
    if (empty()) {
        return false;
    }

    front_ = (front_ + 1) % capacity_;
    --size_;

    // 如果删除后队列为空，则将队头和队尾都重置为 0。
    if (size_ == 0) {
        front_ = 0;
        rear_ = 0;
    }
    return true;
}

// 读取队头元素但不删除；若队列为空则返回 false。
bool MyQueue::retrieve(char& value) const {
    if (empty()) {
        return false;
    }

    value = data_[front_];
    return true;
}

// 返回当前队列中的元素个数。
std::size_t MyQueue::size() const {
    return size_;
}

// 判断当前队列是否为空。
bool MyQueue::empty() const {
    return size_ == 0;
}

// 清空队列：逻辑上直接重置队头、队尾和元素个数即可。
void MyQueue::clear() {
    front_ = 0;
    rear_ = 0;
    size_ = 0;
}

// 将队列内容按“队头到队尾”的顺序拼接成字符串，元素之间用空格分隔。
std::string MyQueue::toString() const {
    std::ostringstream output;
    for (std::size_t i = 0; i < size_; ++i) {
        if (i > 0) {
            output << ' ';
        }
        output << data_[(front_ + i) % capacity_];
    }
    return output.str();
}
