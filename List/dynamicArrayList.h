#pragma once
#include <stdexcept>
#include "virtualList.h"
#define basicCapacity 16

template<typename T>
class dynamicArrayList : public List<T> {
private:
    int m_size;
    int m_capacity;
    T* dynamicArray;

    //Helper Method;
    bool isIndexValid(const int& index) const {
        return index < m_size && index >= 0;
    }

    void resize(bool doubleCapacity = true) {
        if (doubleCapacity) {
            m_capacity *= 2;
        } else if (m_capacity > basicCapacity) {
            m_capacity /= 2;
        }
        T* newArray = new T[m_capacity];
        for (int i = 0; i < m_size; i++) {
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
        for (int i = 0; i < m_size; i++) {
            dynamicArray[i] = other.dynamicArray[i];
        }
    }

    dynamicArrayList& operator=(const dynamicArrayList& other) {
        if (this == &other) {
            return *this;
        }

        delete[] dynamicArray;

        m_size = other.m_size;
        m_capacity = other.m_capacity;
        dynamicArray = new T[m_capacity];
        for (int i = 0; i < m_size; i++) {
            dynamicArray[i] = other.dynamicArray[i];
        }

        return *this;
    }
    ~dynamicArrayList() {
        delete[] dynamicArray;
    }


    int size() const override { return this->m_size; }

    bool isEmpty() const override { return this->m_size == 0; }

    T get(int index) const override {
        if (!isIndexValid(index)) {
            throw std::out_of_range("Index out of bounds");
        }
        return dynamicArray[index];
    }

    int indexOf(const T& value) const override {
        for (int i = 0; i < this->m_size; i++) {
            if (dynamicArray[i] == value) { return i; }
        }
        return -1; 
    }

    bool contains(const T& value) const override {
        return indexOf(value) != -1;
    }


    void set(int index, const T& value) override {
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

    void insert(int index, const T& value) override {
        if (index < 0 || index > m_size) { 
            throw std::out_of_range("Index out of bounds");
        }

        if (m_size == m_capacity) {
            m_capacity *= 2;
            T* newArray = new T[m_capacity];

            for (int i = 0; i < index; i++) {
                newArray[i] = dynamicArray[i];
            }

            newArray[index] = value;

            for (int i = index; i < m_size; i++) {
                newArray[i + 1] = dynamicArray[i];
            }

            delete[] dynamicArray;
            dynamicArray = newArray;
        } else {
            for (int i = m_size; i > index; i--) {
                dynamicArray[i] = dynamicArray[i - 1];
            }
            dynamicArray[index] = value;
        }

        m_size++; 
    }

    void remove(int index) override {
        if (!isIndexValid(index)) {
            throw std::out_of_range("Index out of bounds");
        }

        for (int i = index; i < m_size - 1; i++) {
            dynamicArray[i] = dynamicArray[i + 1];
        }
        m_size--;

        if (m_size > 0 && m_size == m_capacity / 4) {
            resize(false);
        }
    }

    void removeValue(const T& value) override {
        int index = indexOf(value);
        if (index != -1) {
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