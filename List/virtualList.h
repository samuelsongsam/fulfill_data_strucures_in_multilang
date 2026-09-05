#pragma once
#include <cstddef>
#include "iterator.h"
template <typename T>
class Iterator;

template <typename T>
class List {
public:
    static constexpr size_t npos = static_cast<size_t>(-1);

    virtual ~List() = default;

    virtual size_t size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual T get(size_t index) const = 0;
    virtual size_t indexOf(const T& value) const = 0;
    virtual bool contains(const T& value) const = 0;

    virtual void set(size_t index, const T& value) = 0;
    virtual void add(const T& value) = 0;
    virtual void insert(size_t index, const T& value) = 0;
    virtual void remove(size_t index) = 0;
    virtual void removeValue(const T& value) = 0;
    virtual void clear() = 0;

    virtual Iterator<T> iterator() = 0;
};