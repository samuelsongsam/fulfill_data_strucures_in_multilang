#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "virtualList.h"

template<typename T>
class dynamicArrayList : public List<T> {
private:
    static constexpr size_t basicCapacity = 16;
    size_t m_size;
    size_t m_capacity;
    T* dynamicArray;

public:
    //Helper Method;
    void swap(dynamicArrayList& other) noexcept {
        using std::swap;
        swap(m_size, other.m_size);
        swap(m_capacity, other.m_capacity);
        swap(dynamicArray, other.dynamicArray);
    }

    friend void swap(dynamicArrayList& a, dynamicArrayList& b) noexcept {
        a.swap(b);
    }

private:
    bool isIndexValid(const size_t& index) const {
        return index < m_size;
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
    dynamicArrayList() {
        m_size = 0;
        m_capacity = basicCapacity;
        dynamicArray = new T[m_capacity];
    }
    dynamicArrayList(const dynamicArrayList& other) {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        dynamicArray = new T[m_capacity];
        for (size_t i = 0; i < m_size; i++) {
            dynamicArray[i] = other.dynamicArray[i];
        }
    }

    dynamicArrayList& operator=(dynamicArrayList other) noexcept {
        swap(other);
        return *this;
    }
    ~dynamicArrayList() {
        delete[] dynamicArray;
    }


    size_t size() const override { return this->m_size; }

    bool isEmpty() const override { return this->m_size == 0; }

    T get(size_t index) const override {
        if (!isIndexValid(index)) {
            throw std::out_of_range("Index out of bounds");
        }
        return dynamicArray[index];
    }

    size_t indexOf(const T& value) const override {
        for (size_t i = 0; i < this->m_size; i++) {
            if (dynamicArray[i] == value) { return i; }
        }
        return List<T>::npos;
    }

    bool contains(const T& value) const override {
        return indexOf(value) != List<T>::npos;
    }


    void set(size_t index, const T& value) override {
        if (!isIndexValid(index)) {
            throw std::out_of_range("Index out of bounds");
        }
        dynamicArray[index] = value;
    }

    void add(const T& value) override {
        if (m_size == m_capacity)
            resize();
        dynamicArray[m_size] = value;
        m_size++;
    }

    void insert(size_t index, const T& value) override {
        if (index > m_size) {
            throw std::out_of_range("Index out of bounds");
        }

        if (m_size == m_capacity) {
            m_capacity *= 2;
            T* newArray = new T[m_capacity];

            for (size_t i = 0; i < index; i++) {
                newArray[i] = dynamicArray[i];
            }

            newArray[index] = value;

            for (size_t i = index; i < m_size; i++) {
                newArray[i + 1] = dynamicArray[i];
            }

            delete[] dynamicArray;
            dynamicArray = newArray;
        } else {
            for (size_t i = m_size; i > index; i--) {
                dynamicArray[i] = dynamicArray[i - 1];
            }
            dynamicArray[index] = value;
        }

        m_size++; 
    }

    void remove(size_t index) override {
        if (!isIndexValid(index)) {
            throw std::out_of_range("Index out of bounds");
        }

        for (size_t i = index; i < m_size - 1; i++) {
            dynamicArray[i] = dynamicArray[i + 1];
        }
        m_size--;

        if (m_size > 0 && m_size == m_capacity / 4) {
            resize(false);
        }
    }

    void removeValue(const T& value) override {
        size_t index = indexOf(value);
        if (index != List<T>::npos) {
            remove(index);
        }
    }

    void clear() override {
        m_size = 0;
    }

    Iterator<T> iterator() override {
        return Iterator<T>(dynamicArray);
    }

};