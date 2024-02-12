#pragma once

#include "CCircularBuffer.h"

template <typename T>
class CCircularBufferExp final : public CCircularBuffer<T> {
    using CCircularBuffer<T>::buffer_;
    using CCircularBuffer<T>::size_;
    using CCircularBuffer<T>::begin_;
    using CCircularBuffer<T>::end_;
    using CCircularBuffer<T>::capacity_;
public:
    using Iterator = CCircularBuffer<T>::Iterator;

    CCircularBufferExp() : CCircularBuffer<T>() {};
    explicit CCircularBufferExp(size_t buffer_size) : CCircularBuffer<T>(buffer_size) {};
    explicit CCircularBufferExp(size_t buffer_size, const T& value) : CCircularBuffer<T>(buffer_size, value) {};

    Iterator insert(Iterator& pointer, const T& value) override {
        if (size_ != capacity_) {
             return CCircularBuffer<T>::insert(pointer, value);
        } else {
            T* new_buffer = new T[capacity_* 2 + 1];
            size_t it = 0;
            auto begin_it = CCircularBuffer<T>::begin();

            while (begin_it != pointer) {
                new_buffer[it] = *begin_it;
                it++;
                ++begin_it;
            }

            size_t new_pointer = it;
            new_buffer[it] = value;

            for (size_t i = it; i < capacity_; i++) {
                new_buffer[i + 1] = buffer_[(begin_ + i) % (capacity_ + 1)];
            }

            delete[] buffer_;
            buffer_ = new_buffer;
            size_++;
            begin_ = 0;
            end_ = capacity_ + 1;
            capacity_ *= 2;

            return Iterator(buffer_, capacity_, new_pointer, begin_);
        }
    }

    void push_back(const T& value) override {
        if (size_ != capacity_) {
            CCircularBuffer<T>::push_back(value);
        } else {
            T* new_buffer = new T[capacity_* 2 + 1];
            new_buffer[capacity_] = value;

            for (size_t i = 0; i < capacity_; i++) {
                new_buffer[i] = buffer_[(begin_ + i) % (capacity_ + 1)];
            }

            delete[] buffer_;
            buffer_ = new_buffer;
            size_++;
            begin_ = 0;
            end_ = capacity_ + 1;
            capacity_ *= 2;
        }
    }

    void push_front(const T& value) override {
        if (size_ != capacity_) {
            CCircularBuffer<T>::push_front(value);
        } else {
            T* new_buffer = new T[capacity_* 2 + 1];
            new_buffer[0] = value;

            for (size_t i = 0; i < capacity_; i++) {
                new_buffer[i + 1] = buffer_[(begin_ + i) % (capacity_ + 1)];
            }

            delete[] buffer_;
            buffer_ = new_buffer;
            size_++;
            begin_ = 0;
            end_ = capacity_ + 1;
            capacity_ *= 2;
        }
    }
};