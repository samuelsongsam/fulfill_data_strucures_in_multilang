#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "virtualQueue.h"

template <typename T>
class circleDynamicArrayQueue : public Queue<T> {
private:
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr size_t MAX_CAPACITY = 1000000;

    size_t m_size;
    size_t m_capacity;
    T* dynamicArray;
    size_t head;

public:
    void swap(circleDynamicArrayQueue& other) noexcept {
        using std::swap;
        swap(m_size, other.m_size);
        swap(m_capacity, other.m_capacity);
        swap(dynamicArray, other.dynamicArray);
        swap(head, other.head);
    }

    friend void swap(circleDynamicArrayQueue& a, circleDynamicArrayQueue& b) noexcept {
        a.swap(b);
    }

private:
    size_t tail() const {
        return (head + m_size) % m_capacity;
    }

    void resize(bool larger = true) {
        if (m_capacity == DEFAULT_CAPACITY && !larger) {
            return;
        }
        if (larger && m_capacity >= MAX_CAPACITY) {
            throw std::length_error("Queue capacity exceeds maximum limit");
        }
        if (m_size == 0) {
            m_capacity = DEFAULT_CAPACITY;
            delete[] dynamicArray;
            dynamicArray = new T[m_capacity];
            head = 0;
            return;
        }

        size_t oldCapacity = m_capacity;
        if (larger) {
            m_capacity *= 2;
        } else {
            m_capacity /= 2;
        }

        T* newArray = new T[m_capacity];
        for (size_t i = 0; i < m_size; i++) {
            newArray[i] = dynamicArray[(head + i) % oldCapacity];
        }

        delete[] dynamicArray;
        dynamicArray = newArray;
        head = 0;
    }

public:
    circleDynamicArrayQueue() 
        : m_size(0), m_capacity(DEFAULT_CAPACITY), dynamicArray(new T[DEFAULT_CAPACITY]), head(0) {}

    circleDynamicArrayQueue(const circleDynamicArrayQueue& other) 
        : m_size(other.m_size), m_capacity(other.m_capacity), dynamicArray(new T[other.m_capacity]), head(0) {
        for (size_t i = 0; i < m_size; i++) {
            dynamicArray[i] = other.dynamicArray[(other.head + i) % other.m_capacity];
        }
    }

    circleDynamicArrayQueue& operator=(circleDynamicArrayQueue other) noexcept {
        swap(other);
        return *this;
    }

    ~circleDynamicArrayQueue() override {
        delete[] dynamicArray;
    }

    void enqueue(const T& value) override {
        if (m_size == m_capacity) {
            resize(true);
        }
        dynamicArray[tail()] = value;
        m_size++;
    }

    T dequeue() override {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        T value = dynamicArray[head];
        head = (head + 1) % m_capacity;
        m_size--;

        if (m_size > 0 && m_size == m_capacity / 4 && m_capacity > DEFAULT_CAPACITY) {
            resize(false);
        }
        return value;
    }

    T front() const override {
        if (isEmpty()) {
            throw std::out_of_range("Queue is empty");
        }
        return dynamicArray[head];
    }

    size_t size() const override {
        return m_size;
    }

    bool isEmpty() const override {
        return m_size == 0;
    }

    void clear() override {
        m_size = 0;
        head = 0;
    }
};