#pragma once

#include <iostream>

template <typename T>
class CCircularBuffer {
protected:
    T* buffer_;
    size_t size_;
    size_t begin_;
    size_t end_;
    size_t capacity_;

public:
    class Iterator {
    protected:
        T* buffer_;
        size_t capacity_;
        size_t index_;
        size_t begin_;

        [[nodiscard]] size_t getIndex() const {
            return (begin_ + index_) % (capacity_ + 1);
        }
    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        Iterator(T* buffer, size_t capacity, size_t index, size_t begin)
                : buffer_(buffer), capacity_(capacity), index_(index), begin_(begin) {};

        reference operator[](size_t num) {
            return buffer_[(begin_ + index_ + num) % (capacity_ + 1)];
        }

        reference operator*() {
            return buffer_[getIndex()];
        }

        pointer operator->() {
            return buffer_ + getIndex();
        }

        Iterator& operator++() {
            index_++;
            return *this;
        }

        Iterator& operator--() {
            index_--;
            return *this;
        }

        bool operator== (const Iterator& other) const {
            return getIndex() == other.getIndex();
        }

        bool operator!= (const Iterator& other) const {
            return getIndex() != other.getIndex();
        }

        bool operator<= (const Iterator& other) const {
            return getIndex() <= other.getIndex();
        }

        bool operator< (const Iterator& other) const {
            return getIndex() < other.getIndex();
        }

        bool operator>= (const Iterator& other) const {
            return getIndex() >= other.getIndex();
        }

        bool operator> (const Iterator& other) const {
            return getIndex() > other.getIndex();
        }

        Iterator& operator+= (difference_type num) {
            index_ += num;
            return *this;
        }

        Iterator operator+ (difference_type num) const {
            Iterator iterator = *this;
            iterator += num;
            return iterator;
        }

        Iterator& operator-= (difference_type num) {
            index_ -= num;
            return *this;
        }

        Iterator operator- (difference_type num) const {
            Iterator iterator = *this;
            iterator -= num;
            return iterator;
        }

        difference_type operator- (const Iterator& other) const {
            return (index_ % (capacity_ + 1)) - (other.index_ % (other.capacity_ + 1));
        }
    };

    CCircularBuffer() {
        size_ = 0;
        begin_ = end_ = 0;
        capacity_ = 0;
        buffer_ = nullptr;
    }

    explicit CCircularBuffer(size_t buffer_size) {
        size_ = 0;
        begin_ = end_ = 0;
        capacity_ = buffer_size;
        buffer_ = new T[capacity_ + 1];
    }

    explicit CCircularBuffer(size_t buffer_size, const T& value) {
        size_ = buffer_size;
        begin_ = 0;
        end_ = buffer_size;
        capacity_ = buffer_size;
        buffer_ = new T[capacity_ + 1];

        for (size_t i = 0; i < buffer_size; i++) {
            buffer_[i] = value;
        }
    }

    ~CCircularBuffer() {
        delete[] buffer_;
        buffer_ = nullptr;
    }

    CCircularBuffer(const CCircularBuffer& other) {
        size_ = other.size_;
        begin_ = other.begin_;
        end_ = other.end_;
        capacity_ = other.capacity_;
        buffer_ = new T[capacity_ + 1];

        for (size_t i = 0; i <= capacity_; i++) {
            buffer_[i] = other.buffer_[i];
        }
    }

    CCircularBuffer& operator= (const CCircularBuffer& other) {
        if (*this != other) {
            size_ = other.size_;
            begin_ = other.begin_;
            end_ = other.end_;
            capacity_ = other.capacity_;
            delete[] buffer_;
            buffer_ = new T[capacity_ + 1];

            for (size_t i = 0; i <= capacity_; i++) {
                buffer_[i] = other.buffer_[i];
            }
        }

        return *this;
    }

    bool operator== (const CCircularBuffer& rhs) {
        return std::equal(this->begin(), this->end(), rhs.begin(), rhs.end());
    }

    bool operator!= (const CCircularBuffer& rhs) {
        return !std::equal(this->begin(), this->end(), rhs.begin(), rhs.end());
    }

    T& operator[] (size_t num) {
        return buffer_[(begin_ + num) % (capacity_ + 1)];
    }

    virtual Iterator insert(Iterator& pointer, const T& value) {
        if (pointer == begin()) {
            push_front(value);
            return begin();
        }

        T value_from_prev = *pointer;
        *pointer = value;

        Iterator begin = pointer;
        Iterator end = Iterator(buffer_, capacity_, size_, begin_);

        if (begin > end) {
            throw std::out_of_range("In function insert you pointer is out of range");
        }

        while (begin != end) {
            T temp = *(begin + 1);
            *(begin + 1) = value_from_prev;
            value_from_prev = temp;
            ++begin;
        }

        end_ = (end_ + 1) % (capacity_ + 1);

        if (begin_ == end_ && begin_ == capacity_) {
            begin_ = 0;
        } else if (begin_ == end_) {
            begin_++;
        } else {
            size_++;
        }

        return pointer;
    }

    Iterator erase(Iterator& pointer) {
        Iterator begin = pointer;
        Iterator end = Iterator(buffer_, capacity_, size_, begin_);

        if (empty() || begin >= end) {
            return end;
        }

        while (begin != end) {
            *begin = *(begin + 1);
            ++begin;
        }

        size_--;
        end_ = (capacity_ + end_ - 1) % (capacity_ + 1);

        return pointer;
    }

    Iterator erase(Iterator& begin, Iterator& end) {
        if (empty() || begin >= end) {
            return end;
        }

        Iterator temp = begin;
        Iterator temp_end = Iterator(buffer_, capacity_, size_, begin_);
        size_t counter = end - begin;

        while (end != temp_end) {
            *begin = *end;
            ++begin;
            ++end;
        }

        if (size_ - counter < 0) {
            size_ = 0;
        } else {
            size_ -= counter;
        }

        end_ = (begin_ + size_) % (capacity_ + 1);

        return temp;
    }

    void clear() {
        delete[] buffer_;
        buffer_ = nullptr;
        size_ = 0;
        begin_ = 0;
        end_ = 0;
    }

    virtual void push_back(const T& value) {
        buffer_[end_] = value;
        if (end_ == capacity_) {
            end_ = 0;
        } else {
            end_++;
        }

        if (begin_ == end_ && begin_ == capacity_) {
            begin_ = 0;
        } else if (begin_ == end_) {
            begin_++;
        } else {
            size_++;
        }
    }

    void pop_back() {
        if (!empty()) {
            size_--;
            if (end_ == 0) {
                end_ = capacity_;
            } else {
                end_--;
            }
        }
    }

    virtual void push_front(const T& value) {
        if (begin_ == 0) {
            begin_ = capacity_;
            end_--;
        } else if (end_ == 0) {
            begin_--;
            end_ = capacity_;
        } else {
            begin_--;
            end_--;
        }

        if (size_ != capacity_)
            size_++;

        buffer_[begin_] = value;
    }

    void pop_front() {
        if (!empty()) {
            size_--;
            if (begin_ == capacity_) {
                begin_ = 0;
            } else {
                begin_++;
            }
        }
    }

    [[nodiscard]] Iterator begin() const {
        return Iterator(buffer_, capacity_, 0, begin_);
    }

    Iterator begin() {
        return Iterator(buffer_, capacity_, 0, begin_);
    }

    Iterator cbegin() {
        return const_cast<const CCircularBuffer*>(this)->begin();
    }

    [[nodiscard]] Iterator end() const {
        return Iterator(buffer_, capacity_, size_, begin_);
    }

    Iterator end() {
        return Iterator(buffer_, capacity_, size_, begin_);
    }

    Iterator cend() {
        return const_cast<const CCircularBuffer*>(this)->end();
    }

    inline bool empty() {
        return size_ == 0;
    }

    inline size_t size() {
        return size_;
    }

    inline size_t max_size() {
        return capacity_;
    }

    inline T front() {
        return buffer_[begin_];
    }

    T back() {
        return buffer_[end_ - 1];
    }
};

