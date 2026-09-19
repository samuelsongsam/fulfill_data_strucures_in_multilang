#pragma once
#include <cstddef>
template <typename T>
class Deque {
public:
    virtual ~Deque() = default;

    Deque& operator=(const Deque& other) {
        if (this != &other) {
            clear();
            other.copyTo(*this);
        }
        return *this;
    }
    static constexpr size_t npos = static_cast<size_t>(-1);
    virtual void addFirst(const T& item) = 0;
    virtual void addLast(const T& item) = 0;
    virtual T removeFirst() = 0;
    virtual T removeLast() = 0;
    virtual T peekFirst() const = 0;
    virtual T peekLast() const = 0;
    virtual bool isEmpty() const = 0;
    virtual size_t size() const = 0;
    virtual void clear() = 0;

protected:
    virtual void copyTo(Deque& destination) const = 0;
};