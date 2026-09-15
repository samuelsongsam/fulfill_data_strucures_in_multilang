#pragma once
#include <cstddef>
#include <memory>
#include "virtualList.h"

class String {
public:
    static constexpr size_t npos = static_cast<size_t>(-1);

    virtual ~String() = default;

    virtual size_t length() const = 0;
    virtual char charAt(size_t index) const = 0;

    virtual std::unique_ptr<String> substring(size_t start, size_t end) const = 0;
    virtual std::unique_ptr<String> concat(const String& other) const = 0;
    virtual std::unique_ptr<String> replace(const String& target, const String& replacement) const = 0;

    virtual size_t indexOf(const String& other) const = 0;
    virtual bool contains(const String& other) const = 0;
    virtual bool equals(const String& other) const = 0;

    virtual void split(const String& delimiter, List<std::unique_ptr<String>>& result) const = 0;
};