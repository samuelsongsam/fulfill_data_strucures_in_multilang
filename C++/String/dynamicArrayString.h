#pragma once
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <utility>
#include "virtualString.h"
#include "dynamicArrayList.h"

class dynamicArrayString : public String {
private:
    dynamicArrayList<char> m_data;

public:
    dynamicArrayString() = default;

    explicit dynamicArrayString(const char* str) {
        if (str) {
            while (*str != '\0') {
                m_data.add(*str);
                ++str;
            }
        }
    }

    ~dynamicArrayString() override = default;

    size_t length() const override {
        return m_data.size();
    }

    char charAt(size_t index) const override {
        if (index >= m_data.size()) {
            throw std::out_of_range("Index out of bounds");
        }
        return m_data.get(index);
    }

    std::unique_ptr<String> substring(size_t start, size_t end) const override {
        if (start > end || end > m_data.size()) {
            throw std::out_of_range("Invalid substring range");
        }
        auto subStr = std::make_unique<dynamicArrayString>();
        for (size_t i = start; i < end; ++i) {
            subStr->m_data.add(m_data.get(i));
        }
        return subStr;
    }

    std::unique_ptr<String> concat(const String& other) const override {
        auto newStr = std::make_unique<dynamicArrayString>();
        for (size_t i = 0; i < m_data.size(); ++i) {
            newStr->m_data.add(m_data.get(i));
        }
        for (size_t i = 0; i < other.length(); ++i) {
            newStr->m_data.add(other.charAt(i));
        }
        return newStr;
    }

    std::unique_ptr<String> replace(const String& target, const String& replacement) const override {
        auto newStr = std::make_unique<dynamicArrayString>();
        if (target.length() == 0) {
            for (size_t i = 0; i < m_data.size(); ++i) {
                newStr->m_data.add(m_data.get(i));
            }
            return newStr;
        }

        size_t i = 0;
        while (i < m_data.size()) {
            bool match = true;
            if (i + target.length() > m_data.size()) {
                match = false;
            } else {
                for (size_t j = 0; j < target.length(); ++j) {
                    if (m_data.get(i + j) != target.charAt(j)) {
                        match = false;
                        break;
                    }
                }
            }

            if (match) {
                for (size_t j = 0; j < replacement.length(); ++j) {
                    newStr->m_data.add(replacement.charAt(j));
                }
                i += target.length();
            } else {
                newStr->m_data.add(m_data.get(i));
                ++i;
            }
        }
        return newStr;
    }

    size_t indexOf(const String& other) const override {
        if (other.length() == 0) {
            return 0;
        }
        if (other.length() > m_data.size()) {
            return String::npos;
        }

        size_t limit = m_data.size() - other.length();
        for (size_t i = 0; i <= limit; ++i) {
            bool match = true;
            for (size_t j = 0; j < other.length(); ++j) {
                if (m_data.get(i + j) != other.charAt(j)) {
                    match = false;
                    break;
                }
            }
            if (match) {
                return i;
            }
        }
        return String::npos;
    }

    bool contains(const String& other) const override {
        return indexOf(other) != String::npos;
    }

    bool equals(const String& other) const override {
        if (length() != other.length()) {
            return false;
        }
        for (size_t i = 0; i < length(); ++i) {
            if (charAt(i) != other.charAt(i)) {
                return false;
            }
        }
        return true;
    }

    void split(const String& delimiter, List<std::unique_ptr<String>>& result) const override {
        if (delimiter.length() == 0) {
            for (size_t i = 0; i < m_data.size(); ++i) {
                result.add(substring(i, i + 1));
            }
            return;
        }

        size_t start = 0;
        size_t i = 0;
        while (i + delimiter.length() <= m_data.size()) {
            bool match = true;
            for (size_t j = 0; j < delimiter.length(); ++j) {
                if (m_data.get(i + j) != delimiter.charAt(j)) {
                    match = false;
                    break;
                }
            }

            if (match) {
                result.add(substring(start, i));
                i += delimiter.length();
                start = i;
            } else {
                ++i;
            }
        }
        result.add(substring(start, m_data.size()));
    }
};