#ifndef MYQUEUE_H
#define MYQUEUE_H

#include <cstddef>
#include <string>

class MyQueue {
public:
    // 构造一个空队列，并初始化底层循环数组。
    MyQueue();

    // 拷贝构造，注意要深拷贝避免浅拷贝导致动态数组重复释放。
    MyQueue(const MyQueue&);

    // 赋值拷贝，注意要深拷贝避免浅拷贝导致资源管理问题。
    MyQueue& operator=(const MyQueue&);

    // 移动构造函数：接管另一个队列的资源，避免不必要的深拷贝。
    MyQueue(MyQueue&& other) noexcept;

    // 移动赋值运算符：释放当前资源后，接管另一个队列的资源。
    MyQueue& operator=(MyQueue&& other) noexcept;

    // 析构函数，释放底层动态数组占用的内存。
    ~MyQueue();

    // 将一个字符追加到队尾；若数组已满会先自动扩容。
    void append(char value);

    // 删除队头元素；若队列为空则返回 false，否则返回 true。
    bool serve();

    // 读取队头元素到 value 中但不删除；若队列为空返回 false。
    bool retrieve(char& value) const;

    // 返回当前队列中的元素个数。
    std::size_t size() const;

    // 判断队列是否为空；空则返回 true。
    bool empty() const;

    // 清空队列中的所有元素，并将队列状态重置为空。
    void clear();

    // 按队头到队尾的顺序将队列内容转换为字符串，便于输出显示。
    std::string toString() const;

private:
    // 当底层数组容量不足时进行扩容。
    void expand();

    // 指向底层动态数组的指针。
    char* data_;

    // 当前底层数组的总容量。
    std::size_t capacity_;

    // 当前队头元素在循环数组中的下标。
    std::size_t front_;

    // 当前队尾“下一个可插入位置”在循环数组中的下标。
    std::size_t rear_;

    // 当前队列中实际存放的元素个数。
    std::size_t size_;
};

#endif
