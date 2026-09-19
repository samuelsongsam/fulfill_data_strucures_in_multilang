#pragma once
#include <cstddef>
#include "../iterator.h"

template <typename T>
class Set {
public:
    using value_type = T;
    using iterator = Iterator<T>;

    virtual ~Set() = default;

    Set& operator=(const Set& other) {
        if (this != &other) {
            copyFrom(other);
        }
        return *this;
    }

    virtual void add(const T& element) = 0;
    virtual bool remove(const T& element) = 0;
    virtual bool contains(const T& element) const = 0;
    virtual size_t size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void clear() = 0;

    virtual iterator* begin() = 0;
    virtual iterator* end() = 0;

protected:
    virtual void copyFrom(const Set& other) = 0;
};