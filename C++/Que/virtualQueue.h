#pragma once
#include <cstddef>
template <typename T>
class Queue {
public:
    virtual ~Queue() = default;
    virtual void enqueue(const T& value) = 0;
    virtual T dequeue() = 0;
    virtual T front() = 0;
    virtual size_t size() = 0;
    virtual bool isEmpty() = 0;
    virtual void clear() = 0;
};