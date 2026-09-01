#pragma once

template<typename T>
class Iterator {
private:
    T* ptr;

public:
    Iterator(T* p) {
        ptr = p;
    }

    T& operator*() {
        return *ptr;
    }

    Iterator& operator++() {
        ptr++;
        return *this;
    }

    Iterator operator++(int) {
        Iterator temp = *this;
        ptr++;
        return temp;
    }

    bool operator!=(const Iterator& other) const {
        return ptr != other.ptr;
    }

    bool operator==(const Iterator& other) const {
        return ptr == other.ptr;
    }
};