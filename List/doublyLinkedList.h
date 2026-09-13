#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "virtualList.h"

template<typename T>
class doublyLinkedList : public List<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        
        Node() : next(nullptr), prev(nullptr) {}
        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    };

    size_t m_size;
    Node* head;
    Node* tail;

    bool isIndexValid(size_t index) const {
        return index < m_size;
    }

    Node* getNode(size_t index) const {
        if (!isIndexValid(index)) {
            throw std::out_of_range("Index out of bounds");
        }
        
        if (index < m_size / 2) {
            Node* current = head->next;
            for (size_t i = 0; i < index; i++) {
                current = current->next;
            }
            return current;
        } else {
            Node* current = tail->prev;
            size_t steps = (m_size - 1) - index;
            for (size_t i = 0; i < steps; i++) {
                current = current->prev;
            }
            return current;
        }
    }

    void init() {
        m_size = 0;
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }

public:
    void swap(doublyLinkedList& other) noexcept {
        using std::swap;
        swap(m_size, other.m_size);
        swap(head, other.head);
        swap(tail, other.tail);
    }

    friend void swap(doublyLinkedList& a, doublyLinkedList& b) noexcept {
        a.swap(b);
    }

    doublyLinkedList() {
        init();
    }

    doublyLinkedList(const doublyLinkedList& other) {
        init();
        Node* current = other.head->next;
        while (current != other.tail) {
            add(current->data);
            current = current->next;
        }
    }

    doublyLinkedList& operator=(doublyLinkedList other) noexcept {
        swap(other);
        return *this;
    }

    ~doublyLinkedList() override {
        clear();
        delete head;
        delete tail;
    }

    size_t size() const override {
        return m_size; 
    }

    bool isEmpty() const override { 
        return m_size == 0; 
    }

    T get(size_t index) const override {
        return getNode(index)->data;
    }

    size_t indexOf(const T& value) const override {
        Node* current = head->next;
        size_t index = 0;
        while (current != tail) {
            if (current->data == value) {
                return index;
            }
            current = current->next;
            index++;
        }
        return List<T>::npos;
    }

    bool contains(const T& value) const override {
        return indexOf(value) != List<T>::npos;
    }

    void set(size_t index, const T& value) override {
        Node* current = getNode(index);
        current->data = value;
    }

    void add(const T& value) override {
        Node* newNode = new Node(value);
        Node* prevNode = tail->prev;

        prevNode->next = newNode;
        newNode->prev = prevNode;
        newNode->next = tail;
        tail->prev = newNode;

        m_size++;
    }

    void insert(size_t index, const T& value) override {
        if (index > m_size) {
            throw std::out_of_range("Index out of bounds");
        }

        Node* current = (index == m_size) ? tail : getNode(index);
        Node* prevNode = current->prev;
        Node* newNode = new Node(value);

        prevNode->next = newNode;
        newNode->prev = prevNode;
        newNode->next = current;
        current->prev = newNode;

        m_size++;
    }

    void remove(size_t index) override {
        Node* nodeToRemove = getNode(index);
        Node* prevNode = nodeToRemove->prev;
        Node* nextNode = nodeToRemove->next;

        prevNode->next = nextNode;
        nextNode->prev = prevNode;

        delete nodeToRemove;
        m_size--;
    }

    void removeValue(const T& value) override {
        size_t index = indexOf(value);
        if (index != List<T>::npos) {
            remove(index);
        }
    }

    void clear() override {
        Node* current = head->next;
        while (current != tail) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head->next = tail;
        tail->prev = head;
        m_size = 0;
    }

    Iterator<T> iterator() override {
        return Iterator<T>(head->next);
    }
};