#pragma once
#include <stdexcept>
#include "virtualList.h"

template<typename T>
class Node {
public:
    T value;
    Node* next;
    Node() {
        value = 0;
        next = nullptr;
    }
    Node(T v) {
        value = v;
        next = nullptr;
    }
};

template<typename T>
class singlyLinkedArrayList : public List<T> {
private:
    Node<T>* sentinal;
    int m_size;

public:
    singlyLinkedArrayList() {
        sentinal = new Node();
        m_size = 0;
    } 
    singlyLinkedArrayList(T value) {
        sentinal -> next = new Node(value);
        m_size = 1;
    }
    singlyLinkedArrayList(const singlyLinkedArrayList& other) {
        sentinal = new Node<T>();
        m_size = 0;
        Node<T>* current = other.sentinal->next;
        while (current != nullptr) {
            add(current->value);
            current = current->next;
        }
    }
    singlyLinkedArrayList& operator=(const singlyLinkedArrayList& other) {
        if (this == &other) {
            return *this;
        }

        clear();

        Node<T>* current = other.sentinal->next;
        while (current != nullptr) {
            add(current->value);
            current = current->next;
        }

        return *this;
    }
    ~singlyLinkedArrayList() {
        clear();
        delete sentinal;
    }

    int size() const override { return this->m_size; }

    bool isEmpty() const override { return this->m_size == 0; }

    T get(int index) const override {
        if (index < 0 || index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = sentinal;
        for (int i = 1; i <= index; ++i) {
            current = current->next;
        }
        return current->value;
    }

    int indexOf(const T& value) const override {
        Node<T>* current = sentinal->next;
        int index = 0;
        while (current != nullptr) {
            if (current->value == value) {
                return index;
            }
            current = current->next;
            ++index;
        }
        return -1;
    }

    bool contains(const T& value) const override {
        return indexOf(value) != -1;
    }


    void set(int index, const T& value) override {
        if (index < 0 || index > m_size) {
            if (index == m_size + 1) {
                add(value);
            } else
                throw std::out_of_range("Index out of range");
        }
        Node<T>* current = sentinal;
        for (int i = 1; i <= index; ++i) {
            current = current->next;
        }
        current->value = value;
    }

    void add(const T& value) override {
        Node<T>* current = sentinal;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = new Node<T>(value);
        ++m_size;
    }

    void insert(int index, const T& value) override {
        Node<T>* current = sentinal;
        for (int i = 1; i < index; ++i) {
            current = current->next;
        }
        Node<T>* newNode = new Node<T>(value);
        newNode->next = current->next;
        current->next = newNode;
        ++m_size;
    }

    void remove(int index) override {
        if (index < 0 || index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = sentinal;
        for (int i = 1; i < index; ++i) {
            current = current->next;
        }
        Node<T>* nodeToRemove = current->next;
        current->next = nodeToRemove->next;
        delete nodeToRemove;
        --m_size;
    }

    void removeValue(const T& value) override {
        Node<T>* current = sentinal;
        while (current->next != nullptr) {
            if (current->next->value == value) {
                Node<T>* nodeToRemove = current->next;
                current->next = nodeToRemove->next;
                delete nodeToRemove;
                --m_size;
                return;
            }
            current = current->next;
        }
    }

    void clear() override {
        Node<T>* current = sentinal->next;
        while (current != nullptr) {
            Node<T>* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        sentinal->next = nullptr;
        m_size = 0;
    }

    Iterator<T> iterator() override {
        return Iterator<T>(sentinal);
    }

};