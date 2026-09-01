#pragma once
#include "iterator.h"
template <typename T>
class Iterator;

template <typename T>
class List {
public:
    virtual ~List() = default;

    virtual int size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual T get(int index) const = 0;
    virtual int indexOf(const T& value) const = 0;
    virtual bool contains(const T& value) const = 0;

    virtual void set(int index, const T& value) = 0;
    virtual void add(const T& value) = 0;
    virtual void insert(int index, const T& value) = 0;
    virtual void remove(int index) = 0;
    virtual void removeValue(const T& value) = 0;
    virtual void clear() = 0;

    virtual Iterator<T> iterator() = 0;
};