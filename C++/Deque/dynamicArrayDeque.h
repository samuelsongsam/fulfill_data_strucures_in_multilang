#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "virtualDeque.h"

template <typename T>
class dynamicArrayDeque : public Deque<T> {
private:
    size_t m_size;
    size_t m_capacity;
    T* dynamicArray;
    size_t head;
    static constexpr size_t basicCapacity = 16;

    //Helper Method
    void resize(bool larger = true) {
        size_t newcapacity = larger ? m_capacity * 2 : m_capacity / 2;
        T* newArray = new T[newcapacity];
        for (size_t i = 0; i < m_size; i++) {
            newArray[i] = dynamicArray[(head + i) % m_capacity];
        }
        delete[] dynamicArray;
        dynamicArray = newArray;
        m_capacity = newcapacity;
        head = 0;
    }
public:
    void swap(dynamicArrayDeque& other) noexcept {
        using std::swap;
        swap(m_size, other.m_size);
        swap(m_capacity, other.m_capacity);
        swap(head, other.head);
        swap(dynamicArray, other.dynamicArray);
    }
    friend void swap(dynamicArrayDeque& a, dynamicArrayDeque& b) noexcept {
        a.swap(b);
    }
    dynamicArrayDeque() {
        m_size = 0;
        m_capacity = basicCapacity;
        dynamicArray = new T[m_capacity];
        head = 0;
    }
    dynamicArrayDeque(const dynamicArrayDeque& other) {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        dynamicArray = new T[m_capacity];
        for (size_t i = 0; i < m_size; i++) {
            dynamicArray[i] = other.dynamicArray[(i + other.head) % other.m_capacity];
        }
        head = 0;
    }
    dynamicArrayDeque& operator=(dynamicArrayDeque other) noexcept {
        swap(other);
        return *this;
    }
    ~dynamicArrayDeque() override {
        delete[] dynamicArray;
    }
    void addFirst(const T& item) override {
        if (m_size == m_capacity) {
            resize();
        }
        head = (head + m_capacity -1) % m_capacity;
        dynamicArray[head] = item;
        m_size++;
    }
    void addLast(const T& item) override {
        if (m_size == m_capacity){
            resize();
        }
        dynamicArray[(head + m_size) % m_capacity] = item;
        m_size++;
    }
    T removeFirst() override {
        if (isEmpty()) {
            throw std::out_of_range("Emtpy Deque!\n");
        }
        if (m_size == m_capacity/4 && m_capacity > basicCapacity) {
            resize(false);
        }
        T res = dynamicArray[head % m_capacity];
        head = (head + 1) % m_capacity;
        m_size--;
        return res;
    }
    T removeLast() override {
        if (isEmpty()) {
            throw std::out_of_range("Emtpy Deque!\n");
        }
        if (m_size == m_capacity/4 && m_capacity > basicCapacity) {
            resize(false);
        }
        T res = dynamicArray[(head + m_size - 1) % m_capacity];
        m_size--;
        return res;
    }
    T peekFirst() const override {
        if (isEmpty()) {
            throw std::out_of_range("Emtpy Deque!\n");
        }
        return dynamicArray[head];
    }
    T peekLast() const override {
        if (isEmpty()) {
            throw std::out_of_range("Empty Deque!\n");
        }
        return dynamicArray[(head + m_size - 1) % m_capacity];
    }
    bool isEmpty() const override {
        return m_size == 0;
    }
    size_t size() const override {
        return m_size;
    }
    void clear() override {
        m_size = 0;
        head = 0;
    }
};