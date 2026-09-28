#pragma once
#include <cstddef>

template <typename T>
class BST {
public:
    virtual BST& operator=(const BST& other) {
        if (this != &other) {
            clear();
            copyTo(other);
        }
        return *this;
    }

    virtual ~BST() = default;

    virtual bool isEmpty() const = 0;
        
    virtual size_t size() const = 0;
        
    virtual size_t height() const = 0;
        
    virtual void insert(const T& value) = 0;
        
    virtual void remove(const T& value) = 0;
        
    virtual bool search(const T& value) const = 0;
        
    virtual void clear() = 0;
    
    virtual void preOrder(std::function<void(const T&)> visit) const = 0;

    virtual void inOrder(std::function<void(const T&)> visit) const = 0;
        
    virtual void postOrder(std::function<void(const T&)> visit) const = 0;
        
    virtual void levelOrder(std::function<void(const T&)> visit) const = 0;

    protected:
    
    virtual void copyTo(const BST& other) = 0;
};