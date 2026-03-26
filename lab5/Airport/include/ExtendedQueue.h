#ifndef AIRPORT_EXTENDED_QUEUE_H
#define AIRPORT_EXTENDED_QUEUE_H

#include <cstddef>
#include <vector>

#include "Types.h"

// 简单循环队列。
// 这里作为 Runway 的成员，分别保存等待降落和等待起飞的飞机。
template <typename Entry>
class ExtendedQueue {
public:
    // 创建一个固定容量的循环队列。
     ExtendedQueue(std::size_t max_size = 1)
        : entries_(max_size > 0 ? max_size : 1),
          capacity_(max_size > 0 ? max_size : 1),
          count_(0),
          front_(0),
          rear_(0) {}

    // 入队：在队尾插入一个元素。
    ErrorCode append(const Entry& item) {
        if (full()) {
            return ErrorCode::overflow;
        }
        entries_[rear_] = item;
        rear_ = (rear_ + 1) % capacity_;
        ++count_;
        return ErrorCode::success;
    }

    // 出队：删除队头元素。
    ErrorCode serve() {
        if (empty()) {
            return ErrorCode::underflow;
        }
        front_ = (front_ + 1) % capacity_;
        --count_;
        return ErrorCode::success;
    }

    // 读队头：取出队头元素的值，但不删除。
    ErrorCode retrieve(Entry& item) const {
        if (empty()) {
            return ErrorCode::underflow;
        }
        item = entries_[front_];
        return ErrorCode::success;
    }

    // 判断队列是否为空。
    bool empty() const {
        return count_ == 0;
    }

    // 判断队列是否已满。
    bool full() const {
        return count_ == capacity_;
    }

    // 当前队列中的元素个数。
    std::size_t size() const {
        return count_;
    }

private:
    // 顺序存储空间。
    std::vector<Entry> entries_;
    // 队列最大容量。
    std::size_t capacity_;
    // 当前元素个数。
    std::size_t count_;
    // 队头下标。
    std::size_t front_;
    // 下一次入队的位置。
    std::size_t rear_;
};

#endif
