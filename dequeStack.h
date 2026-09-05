#pragma once
#include <stdexcept>
#include "virtualStack.h"
#include "doublyLinkedList.h"

template<typename T>
class dequeStack : public Stack<T> {
private:
    doublyLinkedList<T> container;

public:
    dequeStack() = default;

    void push(const T& value) override {
        container.add(value);
    }

    void pop() override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        container.remove(container.size() - 1);
    }

    T peek() const override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return container.get(container.size() - 1);
    }

    size_t size() const override {
        return container.size();
    }

    bool isEmpty() const override {
        return container.isEmpty();
    }

    void clear() override {
        container.clear();
    }
};
