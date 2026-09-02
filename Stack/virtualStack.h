#pragma once
template <typename T>
class Stack {
public:
    virtual ~Stack() = default;

    virtual void push(const T& value) = 0;
    virtual void pop() = 0;
    virtual T peek() const = 0;
    virtual int size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void clear() = 0;
};
