#pragma once

template<typename T>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual T& operator*() = 0;
    virtual Iterator& operator++() = 0;
    virtual bool operator!=(const Iterator& other) const = 0;
    virtual bool operator==(const Iterator& other) const = 0;
};