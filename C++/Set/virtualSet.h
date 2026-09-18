#pragma once
#include <cstddef>
#include "../iterator.h"

template <typename T>
class Set {
public:
    using value_type = T;
    using iterator = Iterator<T>;

    virtual ~Set() = default;

    virtual void add(const T& element) = 0;
    virtual void remove(const T& element) = 0;
    virtual bool contains(const T& element) const = 0;
    virtual size_t size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void clear() = 0;

    virtual iterator* begin() = 0;
    virtual iterator* end() = 0;

    virtual bool equals(const Set<T>& other) const = 0;
    virtual bool containsAll(const Set<T>& other) const = 0;

    virtual Set<T>& unionWith(const Set<T>& other) = 0;
    virtual Set<T>& intersectionWith(const Set<T>& other) = 0;
    virtual Set<T>& differenceWith(const Set<T>& other) = 0;
};