#pragma once
#include <cstddef>
template <typename T>
class Stack {
public:

    virtual ~Stack() = default;

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();
            other.copyTo(*this);
        }
        return *this;
    }

    virtual void push(const T& value) = 0;
    virtual void pop() = 0;
    virtual T peek() const = 0;
    virtual size_t size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void clear() = 0;

protected:
    virtual void copyTo(Stack& destination) const = 0;
};
