#pragma once
#include <stdexcept>
#include <utility>
#include "virtualStack.h"
#define basicCapacity 16

template<typename T>
class dynamicArrayStack : public Stack<T> {
private:
    size_t m_size;
    size_t m_capacity;
    T* dynamicArray;

    //Helper Method;
    void swap(dynamicArrayStack& other) noexcept {
        using std::swap;
        swap(m_size, other.m_size);
        swap(m_capacity, other.m_capacity);
        swap(dynamicArray, other.dynamicArray);
    }

    void resize(bool doubleCapacity = true) {
        if (doubleCapacity) {
            m_capacity *= 2;
        } else if (m_capacity > basicCapacity) {
            m_capacity /= 2;
        }
        T* newArray = new T[m_capacity];
        for (size_t i = 0; i < m_size; i++) {
            newArray[i] = dynamicArray[i];
        }
        delete[] dynamicArray;
        dynamicArray = newArray;
    }


public:
    dynamicArrayStack() {
        m_size = 0;
        m_capacity = basicCapacity;
        dynamicArray = new T[m_capacity];
    }
    dynamicArrayStack(const dynamicArrayStack& other) {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        dynamicArray = new T[m_capacity];
        for (size_t i = 0; i < m_size; i++) {
            dynamicArray[i] = other.dynamicArray[i];
        }
    }

    dynamicArrayStack& operator=(dynamicArrayStack other) noexcept {
        swap(other);
        return *this;
    }
    ~dynamicArrayStack() {
        delete[] dynamicArray;
    }


    size_t size() const override { return this->m_size; }

    bool isEmpty() const override { return this->m_size == 0; }

    void push(const T& value) override {
        if (m_size == m_capacity)
            resize();
        dynamicArray[m_size] = value;
        m_size++;
    }


    void pop() override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        m_size--;
        if (m_size > 0 && m_size == m_capacity / 4) {
            resize(false);
        }
    }

    T peek() const override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return dynamicArray[m_size - 1];
    }

    void clear() override {
        m_size = 0;
    }

};