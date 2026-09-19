#pragma once
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "virtualStack.h"

template<typename T>
class StackNode {
public:
    T value;
    StackNode* next;

    StackNode() : value(T()), next(nullptr) {}
    StackNode(const T& v) : value(v), next(nullptr) {}
    StackNode(const T& v, StackNode* node) : value(v), next(node) {}
};


template<typename T>
class singlyStack : public Stack<T> {
private :
    StackNode<T>* sentinal;
    size_t m_size;

public:
    using Stack<T>::operator=;

    void swap(singlyStack& other) noexcept {
        using std::swap;
        swap(sentinal, other.sentinal);
        swap(m_size, other.m_size);
    }

    friend void swap(singlyStack& a, singlyStack& b) noexcept {
        a.swap(b);
    }

    singlyStack() {
        sentinal = new StackNode<T>();
        m_size = 0;
    }
    singlyStack(const T& v) {
        sentinal = new StackNode<T>();
        sentinal -> next = new StackNode<T>(v);
        m_size = 1;
    }
    singlyStack(const singlyStack& other) {
        sentinal = new StackNode<T>();
        m_size = other.m_size;
        
        StackNode<T>* tail = sentinal;
        StackNode<T>* curOther = other.sentinal->next;
        
        while (curOther != nullptr) {
            tail->next = new StackNode<T>(curOther->value);
            tail = tail->next;
            curOther = curOther->next;
        }
    }

    singlyStack& operator=(singlyStack other) {
        swap(other);
        return *this;
    }
    ~singlyStack() {
        clear();
        delete sentinal;
    }


    void push(const T& value) override {
        sentinal -> next = new StackNode<T>(value, sentinal -> next);
        m_size++;
    }

    void pop() override {
        if (isEmpty()) {
            throw std::runtime_error("Stack is empty");
        }
        StackNode<T>* nodeToRemove = sentinal -> next;
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
        StackNode<T>* current = sentinal -> next;
        while (current != nullptr) {
            StackNode<T>* nextNode = current -> next;
            delete current;
            current = nextNode;
        }
        m_size = 0;
        sentinal->next = nullptr;
    }

protected:
    static void copyNodes(const StackNode<T>* node, Stack<T>& destination) {
        if (node == nullptr) {
            return;
        }
        copyNodes(node->next, destination);
        destination.push(node->value);
    }

    void copyTo(Stack<T>& destination) const override {
        copyNodes(sentinal->next, destination);
    }

public:
    void reverse() {
        StackNode<T>* prev = nullptr;
        StackNode<T>* current = sentinal->next;
        StackNode<T>* nextNode = nullptr;

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