#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>


template <typename T>
class heap {
private:
    T* m_data;
    size_t m_size;
    size_t m_capacity;

    //helper methods
    void swap(heap& other) noexcept {
        using std::swap;
        swap(m_data, other.m_data);
        swap(m_size, other.m_size);
        swap(m_capacity, other.m_capacity);
    }

    void resize(bool doubleCapacity = true) {
        size_t newCapacity = doubleCapacity ? m_capacity * 2 : m_capacity;
        T* newData = new T[newCapacity];
        for (size_t i = 0; i < m_size; ++i) newData[i] = m_data[i];
        delete[] m_data;
        m_data = newData;
        m_capacity = newCapacity;
    }

    void swim(size_t index) {
        while (index > 0) {
            size_t parent = (index - 1) / 2;
            if (m_data[index] < m_data[parent]) {
                std::swap(m_data[index], m_data[parent]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void sink(size_t index) {
        while(1) {
            size_t leftChild = 2 * index + 1;
            size_t rightChild = 2 * index + 2;
            size_t minChild;
            if (leftChild >= m_size) break;
            if (rightChild >= m_size) {
                minChild = leftChild;
            } else {
                minChild = (m_data[leftChild] < m_data[rightChild]) ? leftChild : rightChild;
            }
            if (m_data[index] > m_data[minChild]) {
                std::swap(m_data[index], m_data[minChild]);
                index = minChild;
            } else {
                break;
            }
        }
    }

public:
    heap() : m_data(new T[512]), m_size(0), m_capacity(512) {}

    heap(const heap& other) : m_data(new T[other.m_capacity]), m_size(other.m_size), m_capacity(other.m_capacity) {
        for (size_t i = 0; i < m_size; ++i) {
            m_data[i] = other.m_data[i];
        }
    }

    heap& operator=(heap other) {
        swap(other);
        return *this;
    }

    ~heap() {
        delete[] m_data;
    }

    void push(const T& value) {
        if (m_size == m_capacity) {
            resize();
        }
        m_data[m_size] = value;
        m_size++;
        swim(m_size - 1);
    }

    void pop() {
        if (m_size == 0) {
            throw std::out_of_range("Heap is empty");
        }
        std::swap(m_data[0], m_data[m_size - 1]);
        m_size--;
        sink(0);
        if (m_size < m_capacity / 4 && m_capacity > 512) {
            resize(false);
        }
    }

    size_t size() const {
        return m_size;
    }

    bool isEmpty() const {
        return m_size == 0;
    }

    const T& peek() const {
        if (m_size == 0) {
            throw std::out_of_range("Heap is empty");
        }
        return m_data[0];
    }
};