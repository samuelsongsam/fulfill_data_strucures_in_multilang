#pragma once
#include <cstddef>
template <typename T>
class Queue {
public:
    virtual ~Queue() = default;

    Queue& operator=(const Queue& other) {
        if (this != &other) {
            clear();
            other.copyTo(*this);
        }
        return *this;
    }
    virtual void enqueue(const T& value) = 0;
    virtual T dequeue() = 0;
    virtual T front() const = 0;
    virtual size_t size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void clear() = 0;

protected:
    virtual void copyTo(Queue& destination) const = 0;
};