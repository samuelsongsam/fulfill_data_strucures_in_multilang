#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "virtualQueue.h"

template<typename T>
class circleLinkedQueue : public Queue<T> {
    private:
    struct Node {
        T value;
        Node* next;
        Node* prev;
        
        Node() : next(nullptr), prev(nullptr) {}
        Node(const T& value) : value(value), next(nullptr), prev(nullptr) {}
    };

    size_t m_size;
    Node* head;
    Node* tail;

public:
    using Queue<T>::operator=;

    void swap(circleLinkedQueue& other) noexcept {
        using std::swap;
        swap(m_size, other.m_size);
        swap(head, other.head);
        swap(tail, other.tail);
    }

    friend void swap(circleLinkedQueue& a, circleLinkedQueue& b) noexcept {
        a.swap(b);
    }

    circleLinkedQueue() {
        m_size = 0;
        head = new Node();
        tail = new Node();
        head->next = tail;
        head->prev = tail;
        tail->next = head;
        tail->prev = head;
    }
    
    circleLinkedQueue(const circleLinkedQueue& other) {
        m_size = 0;
        head = new Node();
        tail = new Node();
        head->next = tail;
        head->prev = tail;
        tail->next = head;
        tail->prev = head;

        Node* current = other.head->next;
        while (current != other.tail) {
            enqueue(current->value);
            current = current->next;
        }
    }

    circleLinkedQueue& operator=(circleLinkedQueue other) {
        swap(other);
        return *this;
    }

    ~circleLinkedQueue() override {
        clear();
        delete head;
        delete tail;
    }

    void enqueue(const T& value) override {
        Node* newNode = new Node(value);
        Node* lastNewNode = tail -> prev;
        lastNewNode->next = newNode;
        newNode->prev = lastNewNode;
        newNode->next = tail;
        tail->prev = newNode;
        m_size++;
    }

    T dequeue() override {
        if (isEmpty())
            throw std::runtime_error("Queue is empty");
        Node* firstNode = head->next;
        T result = firstNode -> value;
        head->next = firstNode->next;
        firstNode->next->prev = head;
        delete firstNode;
        m_size--;
        return result;
    }

    T front() const override {
        if (isEmpty())
            throw std::runtime_error("Queue is empty");
        return head->next->value;
    }
    size_t size() const override {
        return m_size;
    }
    bool isEmpty() const override {
        return m_size == 0;
    }
    void clear() override {
        Node* current = head -> next;
        while (current != tail) {
            Node* nextNode = current -> next;
            delete current;
            current = nextNode;
        }
        head->next = tail;
        head->prev = tail;
        tail->next = head;
        tail->prev = head;
        m_size = 0;
    }

protected:
    void copyTo(Queue<T>& destination) const override {
        Node* current = head->next;
        while (current != tail) {
            destination.enqueue(current->value);
            current = current->next;
        }
    }
};