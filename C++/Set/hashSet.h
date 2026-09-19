#pragma once
#include <cstddef>
#include <functional>
#include <stdexcept>
#include "../List/dynamicArrayList.h"
#include "../List/circleLinkedList.h"
#include "virtualSet.h"
#include "../iterator.h"

template <typename T>
class hashSet : public Set<T> {
private:
    circleLinkedList<dynamicArrayList<T>> buckets;
    size_t m_size;
    size_t bucketsCount_;

    class HashSetIterator : public Iterator<T> {
    private:
        hashSet* owner;
        size_t bucketIndex;
        size_t elementIndex;

        void skipEmptyBuckets() {
            while (bucketIndex < owner->bucketsCount_ &&
                   owner->buckets.get(bucketIndex).isEmpty()) {
                ++bucketIndex;
            }
        }

    public:
        HashSetIterator(hashSet* set, size_t bucket, size_t element)
            : owner(set), bucketIndex(bucket), elementIndex(element) {
            skipEmptyBuckets();
        }

        ~HashSetIterator() override = default;

        T& operator*() override {
            return owner->buckets.get(bucketIndex).get(elementIndex);
        }

        Iterator<T>& operator++() override {
            if (bucketIndex == owner->bucketsCount_) {
                return *this;
            }

            ++elementIndex;
            if (elementIndex >= owner->buckets.get(bucketIndex).size()) {
                ++bucketIndex;
                elementIndex = 0;
                skipEmptyBuckets();
            }

            return *this;
        }

        bool operator!=(const Iterator<T>& other) const override {
            const auto* otherIterator =
                dynamic_cast<const HashSetIterator*>(&other);
            return otherIterator == nullptr ||
                   owner != otherIterator->owner ||
                   bucketIndex != otherIterator->bucketIndex ||
                   elementIndex != otherIterator->elementIndex;
        }

        bool operator==(const Iterator<T>& other) const override {
            return !(*this != other);
        }
    };
    // Helper Methods
    void rehash(bool larger = true) {
        if (bucketsCount_ == 0) {
            bucketsCount_ = 16;
        }

        size_t newBucketsCount = larger ? bucketsCount_ * 2 : bucketsCount_ / 2;
        if (newBucketsCount == 0) {
            newBucketsCount = 1;
        }

        circleLinkedList<dynamicArrayList<T>> newBuckets;

        for (size_t i = 0; i < newBucketsCount; ++i) {
            newBuckets.add(dynamicArrayList<T>{});
        }

        for (size_t i = 0; i < bucketsCount_; ++i) {
            dynamicArrayList<T>& bucket = buckets.get(i);
            for (size_t j = 0; j < bucket.size(); ++j) {
                const T& element = bucket.get(j);
                size_t newIndex = std::hash<T>{}(element) % newBucketsCount;
                newBuckets.get(newIndex).add(element);
            }
        }

        buckets = std::move(newBuckets);
        bucketsCount_ = newBucketsCount;
    }
    size_t hashCode(const T& value) const {
        return std::hash<T>{}(value) % bucketsCount_;
    }


public:
    hashSet() : buckets(), m_size(0), bucketsCount_(16) {
        for (size_t i = 0; i < bucketsCount_; ++i) {
            buckets.add(dynamicArrayList<T>{});
        }
    }

    ~hashSet() override = default;

    void add(const T& element) override {
        if (bucketsCount_ * 0.75 < m_size) {
            rehash();
        }

        size_t index = hashCode(element);
        dynamicArrayList<T>& bucket = buckets.get(index);

        if (!bucket.contains(element)) {
            bucket.add(element);
            ++m_size;
        }
    }

    bool remove(const T& element) override {
        size_t index = hashCode(element);
        dynamicArrayList<T>& bucket = buckets.get(index);

        if (!bucket.contains(element)) {
            return false;
        }

        bucket.removeValue(element);
        --m_size;
        if (bucketsCount_ > 16 && m_size < bucketsCount_ * 0.25) {
            rehash(false);
        }
        return true;
    }

    bool contains(const T& element) const override {
        size_t index = hashCode(element);
        return buckets.get(index).contains(element);
    }

    size_t size() const override {
        return m_size;
    }

    bool isEmpty() const override {
        return m_size == 0;
    }

    void clear() override {
        for (size_t i = 0; i < bucketsCount_; ++i) {
            buckets.get(i).clear();
        }
        m_size = 0;
    }

    typename Set<T>::iterator* begin() override {
        return new HashSetIterator(this, 0, 0);
    }
    typename Set<T>::iterator* end() override {
        return new HashSetIterator(this, bucketsCount_, 0);
    }

protected:
    void copyFrom(const Set<T>& other) override {
        clear();
        Set<T>& source = const_cast<Set<T>&>(other);
        auto beginIterator = source.begin();
        auto endIterator = source.end();

        for (; *beginIterator != *endIterator; ++(*beginIterator)) {
            add(**beginIterator);
        }

        delete beginIterator;
        delete endIterator;
    }
};
