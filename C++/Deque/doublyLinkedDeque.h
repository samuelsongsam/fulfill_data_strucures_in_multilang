#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "virtualDeque.h"

template <typename T>
class DoublyLinkedDeque : public Deque<T> {
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

public:
    using Deque<T>::operator=;

    void swap(DoublyLinkedDeque& other) noexcept {
        using std::swap;
        swap(m_size, other.m_size);
        swap(head, other.head);
        swap(tail, other.tail);
    }

    friend void swap(DoublyLinkedDeque& a, DoublyLinkedDeque& b) noexcept {
        a.swap(b);
    }

    DoublyLinkedDeque() {
        m_size = 0;
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
    }
    DoublyLinkedDeque(const DoublyLinkedDeque& other) {
        m_size = 0;
        head = new Node();
        tail = new Node();
        head->next = tail;
        tail->prev = head;
        Node* current = other.head->next;
        while (current != other.tail) {
            addLast(current->data);
            current = current->next;
        }

    }
    DoublyLinkedDeque& operator=(DoublyLinkedDeque other) {
        swap(other);
        return *this;
    }
    ~DoublyLinkedDeque() override {
        clear();
        delete head;
        delete tail;
    }
    void addFirst(const T& item) override {
        Node* newFirst = new Node(item);
        newFirst -> next = head -> next;
        newFirst -> prev = head;
        head -> next -> prev = newFirst;
        head -> next = newFirst;
        m_size++;
    }
    void addLast(const T& item) override {
        Node* newLast = new Node(item);
        newLast -> next = tail;
        newLast -> prev = tail -> prev;
        tail -> prev -> next = newLast;
        tail -> prev = newLast;
        m_size++;
    }
    T removeFirst() override {
        if (isEmpty()) {
            throw std::out_of_range("Empty deque!\n");
        }
        Node* toBeRemoved = head -> next;
        T res = toBeRemoved -> data;
        toBeRemoved -> next -> prev = head;
        head -> next = toBeRemoved -> next;
        delete toBeRemoved;
        m_size--;
        return res;
    }
    T removeLast() override {
        if (isEmpty()) {
            throw std::out_of_range("Empty deque!\n");
        }
        Node* toBeRmoved = tail -> prev;
        T res = toBeRmoved -> data;
        toBeRmoved -> prev -> next = tail;
        tail -> prev = toBeRmoved -> prev;
        delete toBeRmoved;
        m_size--;
        return res;
    }
    T peekFirst() const override {
        if (isEmpty()) {
            throw std::out_of_range("Empty deque!\n");
        }
        return head -> next -> data;
    }
    T peekLast() const override {
        if (isEmpty()) {
            throw std::out_of_range("Empty deque!\n");
        }
        return tail -> prev -> data;
    }
    bool isEmpty() const override {
        return m_size == 0;
    }
    size_t size() const override {
        return m_size;
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

protected:
    void copyTo(Deque<T>& destination) const override {
        Node* current = head->next;
        while (current != tail) {
            destination.addLast(current->data);
            current = current->next;
        }
    }
};