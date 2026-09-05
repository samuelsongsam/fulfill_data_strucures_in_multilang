#pragma once
#include <stdexcept>
#include "virtualStack.h"

template<typename T>
class Node {
public:
    T value;
    Node* next;

    Node() : value(T()), next(nullptr) {}
    Node(const T& v) : value(v), next(nullptr) {}
    Node(const T& v, Node* node) : value(v), next(node) {}
};


template<typename T>
class singlyStack : public Stack<T> {
private :
    Node<T>* sentinal;
    size_t m_size;
public:
    singlyStack() {
        sentinal = new Node<T>();
        m_size = 0;
    }
    singlyStack(const T& v) {
        sentinal = new Node<T>();
        sentinal -> next = new Node<T>(v);
        m_size = 1;
    }
    singlyStack(const singlyStack& other) {
        sentinal = new Node<T>();
        m_size = other.m_size;
        
        Node<T>* tail = sentinal;
        Node<T>* curOther = other.sentinal->next;
        
        while (curOther != nullptr) {
            tail->next = new Node<T>(curOther->value);
            tail = tail->next;
            curOther = curOther->next;
        }
    }

    singlyStack& operator=(const singlyStack& other) {
        if (this == &other) {
            return *this;
        }

        clear();
        this->m_size = other.m_size;
        
        Node<T>* tail = sentinal; 
        Node<T>* curOther = other.sentinal->next;
        
        while (curOther != nullptr) {
            tail->next = new Node<T>(curOther->value);
            tail = tail->next;
            curOther = curOther->next;
        }

        return *this;
    }
    ~singlyStack() {
        clear();
        delete sentinal;
    }


    void push(const T& value) override {
        sentinal -> next = new Node<T>(value, sentinal -> next);
        m_size++;
    }

    void pop() override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        Node<T>* nodeToRemove = sentinal -> next;
        sentinal -> next = nodeToRemove -> next;
        delete nodeToRemove;
        m_size--;
    }

    T peek() const override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        return sentinal -> next -> value;
    }

    size_t size() const override {
        return m_size;
    }

    bool isEmpty() const override {
        return m_size == 0;
    }

    void clear() override {
        Node<T>* current = sentinal -> next;
        while (current != nullptr) {
            Node<T>* nextNode = current -> next;
            delete current;
            current = nextNode;
        }
        m_size = 0;
        sentinal->next = nullptr;
    }

    void reverse() {
        Node<T>* prev = nullptr;
        Node<T>* current = sentinal->next;
        Node<T>* nextNode = nullptr;

        while (current != nullptr) {
            nextNode = current->next; // 先记录下一步要走的位置
            current->next = prev;     // 核心：把当前节点的指针反向指
            prev = current;           // prev 往前跟进
            current = nextNode;       // current 往前跟进
        }
        
        // 此时 prev 指向了原来链表的最后一个节点，它现在成了头节点
        sentinal->next = prev; 
    }
};