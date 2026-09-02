#pragma once
#include <stdexcept>
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

    int m_size;
    Node* head;
    Node* tail;

    bool isIndexValid(const int& index) const {
        return index >= 0 && index < m_size;
    }

    Node* getNode(const int& index) const {
        if (!isIndexValid(index)) {
            throw std::out_of_range("Index out of bounds");
        }
        
        if (index < m_size / 2) {
            Node* current = head->next;
            for (int i = 0; i < index; i++) {
                current = current->next;
            }
            return current;
        } else {
            Node* current = tail->prev;
            for (int i = m_size - 1; i > index; i--) {
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

    doublyLinkedList& operator=(const doublyLinkedList& other) {
        if (this == &other) {
            return *this;
        }
        
        clear();
        Node* current = other.head->next;
        while (current != other.tail) {
            add(current->data);
            current = current->next;
        }
        
        return *this;
    }

    ~doublyLinkedList() {
        clear();
        delete head;
        delete tail;
    }

    int size() const override { 
        return m_size; 
    }

    bool isEmpty() const override { 
        return m_size == 0; 
    }

    T get(int index) const override {
        return getNode(index)->data;
    }

    int indexOf(const T& value) const override {
        Node* current = head->next;
        int index = 0;
        while (current != tail) {
            if (current->data == value) {
                return index;
            }
            current = current->next;
            index++;
        }
        return -1;
    }

    bool contains(const T& value) const override {
        return indexOf(value) != -1;
    }

    void set(int index, const T& value) override {
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

    void insert(int index, const T& value) override {
        if (index < 0 || index > m_size) {
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

    void remove(int index) override {
        Node* nodeToRemove = getNode(index);
        Node* prevNode = nodeToRemove->prev;
        Node* nextNode = nodeToRemove->next;

        prevNode->next = nextNode;
        nextNode->prev = prevNode;

        delete nodeToRemove;
        m_size--;
    }

    void removeValue(const T& value) override {
        int index = indexOf(value);
        if (index != -1) {
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