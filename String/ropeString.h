#pragma once
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <utility>
#include "virtualString.h"
#include "virtualList.h"

class RopeString : public String {
private:
    struct RopeNode {
        size_t weight;
        RopeNode* left;
        RopeNode* right;
        char* data;         

        RopeNode(const char* str, size_t len) 
            : weight(len), left(nullptr), right(nullptr) {
            data = new char[len];
            for (size_t i = 0; i < len; ++i) {
                data[i] = str[i];
            }
        }

        RopeNode(RopeNode* l, RopeNode* r, size_t w) 
            : weight(w), left(l), right(r), data(nullptr) {}

        bool isLeaf() const {
            return left == nullptr && right == nullptr;
        }

        ~RopeNode() {
            delete[] data;
        }
    };

    RopeNode* root;
    size_t m_totalLength;

    static void destroyTree(RopeNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    static RopeNode* copyTree(const RopeNode* node) {
        if (!node) return nullptr;
        if (node->isLeaf()) {
            return new RopeNode(node->data, node->weight);
        }
        RopeNode* newLeft = copyTree(node->left);
        RopeNode* newRight = copyTree(node->right);
        return new RopeNode(newLeft, newRight, node->weight);
    }

    RopeString(RopeNode* r, size_t totalLen) : root(r), m_totalLength(totalLen) {}

    static char charAtNode(const RopeNode* node, size_t index) {
        if (node->isLeaf()) {
            return node->data[index];
        }
        if (index < node->weight) {
            return charAtNode(node->left, index);
        } else {
            return charAtNode(node->right, index - node->weight);
        }
    }

    static void collectChars(const RopeNode* node, char* buffer, size_t& offset) {
        if (!node) return;
        if (node->isLeaf()) {
            for (size_t i = 0; i < node->weight; ++i) {
                buffer[offset++] = node->data[i];
            }
            return;
        }
        collectChars(node->left, buffer, offset);
        collectChars(node->right, buffer, offset);
    }

public:
    RopeString() : root(nullptr), m_totalLength(0) {}

    explicit RopeString(const char* str) : root(nullptr), m_totalLength(0) {
        if (str) {
            size_t len = 0;
            while (str[len] != '\0') ++len;
            if (len > 0) {
                root = new RopeNode(str, len);
                m_totalLength = len;
            }
        }
    }

    RopeString(const RopeString& other) {
        root = copyTree(other.root);
        m_totalLength = other.m_totalLength;
    }

    RopeString& operator=(RopeString other) noexcept {
        std::swap(root, other.root);
        std::swap(m_totalLength, other.m_totalLength);
        return *this;
    }

    ~RopeString() override {
        destroyTree(root);
    }

    size_t length() const override {
        return m_totalLength;
    }

    char charAt(size_t index) const override {
        if (index >= m_totalLength || !root) {
            throw std::out_of_range("Index out of bounds");
        }
        return charAtNode(root, index);
    }

    std::unique_ptr<String> concat(const String& other) const override {
        if (other.length() == 0) {
            return std::make_unique<RopeString>(*this);
        }
        if (this->length() == 0) {
            auto res = std::make_unique<RopeString>();
            for (size_t i = 0; i < other.length(); ++i) {
                // 占位构建
            }
            // 通过字符序列重构
            char* buf = new char[other.length()];
            for (size_t i = 0; i < other.length(); ++i) buf[i] = other.charAt(i);
            res->root = new RopeNode(buf, other.length());
            res->m_totalLength = other.length();
            delete[] buf;
            return res;
        }
        RopeNode* leftTree = copyTree(this->root);

        RopeNode* rightTree = nullptr;
        const RopeString* otherRope = dynamic_cast<const RopeString*>(&other);
        if (otherRope) {
            rightTree = copyTree(otherRope->root);
        } else {
            char* buf = new char[other.length()];
            for (size_t i = 0; i < other.length(); ++i) buf[i] = other.charAt(i);
            rightTree = new RopeNode(buf, other.length());
            delete[] buf;
        }

        RopeNode* newRoot = new RopeNode(leftTree, rightTree, this->m_totalLength);
        return std::unique_ptr<String>(new RopeString(newRoot, this->m_totalLength + other.length()));
    }

    std::unique_ptr<String> substring(size_t start, size_t end) const override {
        if (start > end || end > m_totalLength) {
            throw std::out_of_range("Invalid substring range");
        }
        size_t subLen = end - start;
        if (subLen == 0) {
            return std::make_unique<RopeString>();
        }

        // 从树中直接拉平目标区间并生成新叶子节点
        char* buf = new char[subLen];
        for (size_t i = 0; i < subLen; ++i) {
            buf[i] = this->charAt(start + i);
        }

        RopeNode* subNode = new RopeNode(buf, subLen);
        delete[] buf;
        return std::unique_ptr<String>(new RopeString(subNode, subLen));
    }

    size_t indexOf(const String& other) const override {
        if (other.length() == 0) return 0;
        if (other.length() > m_totalLength) return String::npos;

        size_t limit = m_totalLength - other.length();
        for (size_t i = 0; i <= limit; ++i) {
            bool match = true;
            for (size_t j = 0; j < other.length(); ++j) {
                if (charAt(i + j) != other.charAt(j)) {
                    match = false;
                    break;
                }
            }
            if (match) return i;
        }
        return String::npos;
    }

    bool contains(const String& other) const override {
        return indexOf(other) != String::npos;
    }

    bool equals(const String& other) const override {
        if (length() != other.length()) return false;
        for (size_t i = 0; i < length(); ++i) {
            if (charAt(i) != other.charAt(i)) return false;
        }
        return true;
    }

    std::unique_ptr<String> replace(const String& target, const String& replacement) const override {
        if (target.length() == 0 || m_totalLength == 0) {
            return std::make_unique<RopeString>(*this);
        }

        std::unique_ptr<String> result = std::make_unique<RopeString>();
        size_t i = 0;
        while (i < m_totalLength) {
            bool match = true;
            if (i + target.length() > m_totalLength) {
                match = false;
            } else {
                for (size_t j = 0; j < target.length(); ++j) {
                    if (charAt(i + j) != target.charAt(j)) {
                        match = false;
                        break;
                    }
                }
            }

            if (match) {
                result = result->concat(replacement);
                i += target.length();
            } else {
                char single[1] = { charAt(i) };
                RopeString charStr(single);
                // 截取单字符追加
                auto piece = this->substring(i, i + 1);
                result = result->concat(*piece);
                ++i;
            }
        }
        return result;
    }

    void split(const String& delimiter, List<std::unique_ptr<String>>& result) const override {
        if (delimiter.length() == 0) {
            for (size_t i = 0; i < m_totalLength; ++i) {
                result.add(substring(i, i + 1));
            }
            return;
        }

        size_t start = 0;
        size_t i = 0;
        while (i + delimiter.length() <= m_totalLength) {
            bool match = true;
            for (size_t j = 0; j < delimiter.length(); ++j) {
                if (charAt(i + j) != delimiter.charAt(j)) {
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
        result.add(substring(start, m_totalLength));
    }
};