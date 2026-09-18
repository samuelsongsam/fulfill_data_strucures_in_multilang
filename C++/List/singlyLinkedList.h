#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "virtualList.h"

template<typename T>
class Node {
public:
    T value;
    Node* next;
    Node() : value(T()), next(nullptr) {}
    Node(T v) {
        value = v;
        next = nullptr;
    }
};

template<typename T>
class singlyLinkedListIterator : public Iterator<T> {
private:
    Node<T>* current;

public:
    explicit singlyLinkedListIterator(Node<T>* start) : current(start) {}

    T& operator*() override {
        return current->value;
    }

    Iterator<T>& operator++() override {
        if (current != nullptr) {
            current = current->next;
        }
        return *this;
    }

    bool operator!=(const Iterator<T>& other) const override {
        const auto* otherIt = dynamic_cast<const singlyLinkedListIterator<T>*>(&other);
        return otherIt == nullptr ? true : current != otherIt->current;
    }

    bool operator==(const Iterator<T>& other) const override {
        const auto* otherIt = dynamic_cast<const singlyLinkedListIterator<T>*>(&other);
        return otherIt != nullptr && current == otherIt->current;
    }
};

template<typename T>
class singlyLinkedArrayList : public List<T> {
private:
    Node<T>* sentinal;
    size_t m_size;

    using iterator_impl = singlyLinkedListIterator<T>;

public:
    void swap(singlyLinkedArrayList& other) noexcept {
        using std::swap;
        swap(sentinal, other.sentinal);
        swap(m_size, other.m_size);
    }

    friend void swap(singlyLinkedArrayList& a, singlyLinkedArrayList& b) noexcept {
        a.swap(b);
    }

    singlyLinkedArrayList() {
        sentinal = new Node<T>();
        m_size = 0;
    } 
    singlyLinkedArrayList(T value) {
        sentinal = new Node<T>();
        sentinal -> next = new Node<T>(value);
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
    singlyLinkedArrayList& operator=(singlyLinkedArrayList other) noexcept {
        swap(other);
        return *this;
    }
    ~singlyLinkedArrayList() {
        clear();
        delete sentinal;
    }

    size_t size() const override { return this->m_size; }

    bool isEmpty() const override { return this->m_size == 0; }

    T get(size_t index) const override {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = sentinal;
        for (size_t i = 1; i <= index; ++i) {
            current = current->next;
        }
        return current->value;
    }

    size_t indexOf(const T& value) const override {
        Node<T>* current = sentinal->next;
        size_t index = 0;
        while (current != nullptr) {
            if (current->value == value) {
                return index;
            }
            current = current->next;
            ++index;
        }
        return List<T>::npos;
    }

    bool contains(const T& value) const override {
        return indexOf(value) != List<T>::npos;
    }


    void set(size_t index, const T& value) override {
        if (index >= m_size) {
            if (index == m_size) {
                add(value);
            } else {
                throw std::out_of_range("Index out of range");
            }
        }
        Node<T>* current = sentinal;
        for (size_t i = 1; i <= index; ++i) {
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

    void insert(size_t index, const T& value) override {
        Node<T>* current = sentinal;
        for (size_t i = 1; i < index; ++i) {
            current = current->next;
        }
        Node<T>* newNode = new Node<T>(value);
        newNode->next = current->next;
        current->next = newNode;
        ++m_size;
    }

    void remove(size_t index) override {
        if (index >= m_size) {
            throw std::out_of_range("Index out of range");
        }
        Node<T>* current = sentinal;
        for (size_t i = 1; i < index; ++i) {
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

    typename List<T>::iterator* begin() override {
        return new iterator_impl(sentinal->next);
    }

    typename List<T>::iterator* end() override {
        return new iterator_impl(nullptr);
    }

};