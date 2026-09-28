#pragma once
#include <stdexcept>
#include <cstddef>
#include <utility>
#include <functional>
#include "virtualBST.h"
#include "circleDynamicArrayQueue.h"

template <typename T>
class LLRB : public BST<T> {
private:
    struct Node {
        T value;
        Node* Left;
        Node* Right;
        bool isRed;

        Node(T v = T(), Node* L = nullptr, Node* R = nullptr, bool r = true) 
            : value(v), Left(L), Right(R), isRed(r) {}
    };

    size_t m_size;
    Node* root;

    void swap(LLRB& other) noexcept {
        std::swap(root, other.root);
        std::swap(m_size, other.m_size);
    }

    bool isRed(Node* node) const {
        if (node == nullptr) return false;
        return node->isRed;
    }

    Node* rotateLeft(Node* h) {
        Node* res = h->Right;
        h->Right = res->Left;
        res->Left = h;
        res->isRed = h->isRed;
        h->isRed = true;
        return res;
    }

    Node* rotateRight(Node* h) {
        Node* res = h->Left;
        h->Left = res->Right;
        res->Right = h;
        res->isRed = h->isRed;
        h->isRed = true;
        return res;
    }

    Node* flipColour(Node* h) {
        h->isRed = !h->isRed;
        if (h->Left) h->Left->isRed = !h->Left->isRed;
        if (h->Right) h->Right->isRed = !h->Right->isRed;
        return h;
    }

    Node* fixUp(Node* h) {
        if (isRed(h->Right) && !isRed(h->Left))      h = rotateLeft(h);
        if (isRed(h->Left)  && isRed(h->Left->Left)) h = rotateRight(h);
        if (isRed(h->Left)  && isRed(h->Right))      h = flipColour(h);
        return h;
    }

    Node* moveRedLeft(Node* h) {
        h = flipColour(h);
        if (h->Right != nullptr && isRed(h->Right->Left)) {
            h->Right = rotateRight(h->Right);
            h = rotateLeft(h);
            h = flipColour(h);
        }
        return h;
    }

    Node* moveRedRight(Node* h) {
        h = flipColour(h);
        if (h->Left != nullptr && isRed(h->Left->Left)) {
            h = rotateRight(h);
            h = flipColour(h);
        }
        return h;
    }

    Node* insertHelper(Node* h, const T& value) {
        if (h == nullptr) {
            m_size++;
            return new Node(value);
        }

        if (value < h->value) {
            h->Left = insertHelper(h->Left, value);
        } else if (value > h->value) {
            h->Right = insertHelper(h->Right, value);
        } else {
            return h;
        }

        return fixUp(h);
    }

    Node* removeMin(Node* h) {
        if (h->Left == nullptr) {
            delete h;
            return nullptr;
        }
        if (!isRed(h->Left) && !isRed(h->Left->Left)) {
            h = moveRedLeft(h);
        }
        h->Left = removeMin(h->Left);
        return fixUp(h);
    }

    Node* removeHelper(Node* h, const T& value, bool& deleted) {
        if (h == nullptr) {
            deleted = false;
            return nullptr;
        }

        if (value < h->value) {
            if (h->Left != nullptr && !isRed(h->Left) && !isRed(h->Left->Left)) {
                h = moveRedLeft(h);
            }
            h->Left = removeHelper(h->Left, value, deleted);
        } else {
            if (isRed(h->Left)) {
                h = rotateRight(h);
            }
            
            if (value == h->value && h->Right == nullptr) {
                deleted = true;
                delete h;
                return nullptr;
            }
            
            if (h->Right != nullptr && !isRed(h->Right) && !isRed(h->Right->Left)) {
                h = moveRedRight(h);
            }
            
            if (value == h->value) {
                deleted = true;
                Node* successor = h->Right;
                while (successor->Left != nullptr) successor = successor->Left;
                
                h->value = successor->value;
                h->Right = removeMin(h->Right);
            } else {
                h->Right = removeHelper(h->Right, value, deleted);
            }
        }
        return fixUp(h);
    }

    size_t heightHelper(Node* node) const {
        if (node == nullptr) return 0;
        return 1 + std::max(heightHelper(node->Left), heightHelper(node->Right));
    }

    void clearHelper(Node* node) {
        if (node == nullptr) return;
        clearHelper(node->Left);
        clearHelper(node->Right);
        delete node;
    }

    void preOrderHelper(Node* node, std::function<void(const T&)> visit) const {
        if (node == nullptr) return;
        visit(node->value);
        preOrderHelper(node->Left, visit);
        preOrderHelper(node->Right, visit);
    }

    void inOrderHelper(Node* node, std::function<void(const T&)> visit) const {
        if (node == nullptr) return;
        inOrderHelper(node->Left, visit);
        visit(node->value);
        inOrderHelper(node->Right, visit);
    }

    void postOrderHelper(Node* node, std::function<void(const T&)> visit) const {
        if (node == nullptr) return;
        postOrderHelper(node->Left, visit);
        postOrderHelper(node->Right, visit);
        visit(node->value);
    }

    void levelOrderHelper(Node* node, std::function<void(const T&)> visit) const {
        if (node == nullptr) return;
        circleDynamicArrayQueue<Node*> q;
        q.push(node);
        while (!q.empty()) {
            Node* current = q.front();
            q.pop();
            visit(current->value);
            if (current->Left != nullptr) q.push(current->Left);
            if (current->Right != nullptr) q.push(current->Right);
        }
    }

public:
    LLRB() {
        root = nullptr;
        m_size = 0;
    }

    LLRB(const LLRB& other) {
        root = nullptr;
        m_size = 0;
        other.copyTo(*this);
    }

    LLRB& operator=(LLRB other) {
        swap(other);
        return *this;
    }

    ~LLRB() {
        clear();
    }

    bool isEmpty() const override {
        return m_size == 0;
    }
        
    size_t size() const override {
        return m_size;
    }
        
    size_t height() const override {
        return heightHelper(root);
    }
        
    void insert(const T& value) override {
        root = insertHelper(root, value);
        root->isRed = false;
    }
        
    void remove(const T& value) override {
        bool deleted = false;
        root = removeHelper(root, value, deleted);
        if (deleted) {
            m_size--;
        }
        if (root != nullptr) root->isRed = false;
    }
        
    bool search(const T& value) const override {
        Node* curr = root;
        while (curr != nullptr) {
            if (value < curr->value) {
                curr = curr->Left;
            } else if (value > curr->value) {
                curr = curr->Right;
            } else {
                return true;
            }
        }
        return false;
    }
        
    void clear() override {
        clearHelper(root);
        root = nullptr;
        m_size = 0;
    }
    
    void preOrder(std::function<void(const T&)> visit) const override {
        preOrderHelper(root, visit);
    }

    void inOrder(std::function<void(const T&)> visit) const override {
        inOrderHelper(root, visit);
    }

    void postOrder(std::function<void(const T&)> visit) const override {
        postOrderHelper(root, visit);
    }

    void levelOrder(std::function<void(const T&)> visit) const override {
        levelOrderHelper(root, visit);
    }

protected:
    void copyTo(const BST& other) override {
        const LLRB* otherLLRB = dynamic_cast<const LLRB*>(&other);
        if (otherLLRB) {
            clear();
            std::function<void(const T&)> insertFunc = [this](const T& value) {
                this->insert(value);
            };
            otherLLRB->inOrder(insertFunc);
        }
    }
};