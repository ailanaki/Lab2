#pragma once

#include <cstdlib>
#include <cassert>
#include <exception>
#include <utility>


template<typename T>
class Array final {
public:

    class Iterator;
    class ConstIterator;

    Array() : capacity_{ kDefCapacity_ }, length_{ 0 } {
        create_buf((capacity_) * sizeof(T));
    }

    explicit Array(int capacity) : capacity_{ capacity }, length_{ 0 } {
        if (capacity <= 0) capacity_ = kDefCapacity_;
        create_buf((capacity_) * sizeof(T));
    }

    Array(const Array& other) : capacity_(other.capacity_), length_(0) {
        create_buf((capacity_) * sizeof(T));
        for (int i = 0; i < other.length_; ++i) {
            new (buf_ + i) T(other.buf_[i]);
            length_++;
        }
    }

    Array& operator=(const Array& other) {
        if (this == &other) {
            return *this;
        }

        Array tmp(other);

        std::swap(buf_, tmp.buf_);
        std::swap(length_, tmp.length_);
        std::swap(capacity_, tmp.capacity_);

        return *this;
    }

    Array(Array&& other) noexcept : buf_(other.buf_),
        length_(other.length_),
        capacity_(other.capacity_)
    {
        other.buf_ = nullptr;
        other.length_ = 0;
        other.capacity_ = kDefCapacity_;
    }

    Array& operator=(Array&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        delete_buf();

        buf_ = other.buf_;
        length_ = other.length_;
        capacity_ = other.capacity_;

        other.buf_ = nullptr;
        other.length_ = 0;
        other.capacity_ = kDefCapacity_;

        return *this;
    }

    ~Array() {
        delete_buf();
    }

    int insert(const T& value) {
        check_capacity();

        new (buf_ + length_) T(value);
        return length_++;
    }

    int insert(int index, const T& value) {
        assert(index >= 0 && index <= length_);

        check_capacity();

        for (int i = length_; i > index; i--) {
            new (buf_ + i) T(std::move(buf_[i - 1]));
            buf_[i - 1].~T();
        }

        new (buf_ + index) T(value);
        length_++;
        return index;
    }

    void remove(int index) {
        assert(index >= 0 && index < length_);

        for (int i = index; i < length_ - 1; ++i) {
            buf_[i].~T();
            new (buf_ + i) T(std::move(buf_[i + 1]));
        }

        buf_[--length_].~T();
    }

    const T& operator[](int index) const {
        assert(index >= 0 && index < length_);

        return buf_[index];
    }

    T& operator[](int index) {
        assert(index >= 0 && index < length_);

        return buf_[index];
    }

    int size() const {
        return length_;
    }

    Iterator iterator() {
        if (length_ == 0) {
            return Iterator(nullptr, buf_, buf_, +1);
        }
        return Iterator(buf_, buf_, buf_ + length_, +1);
    }

    ConstIterator iterator() const {
        if (length_ == 0) {
            return ConstIterator(nullptr, buf_, buf_, +1);
        }
        return ConstIterator(buf_, buf_, buf_ + length_, +1);
    }

    Iterator reverseIterator() {
        if (length_ == 0) {
            return Iterator(nullptr, buf_, buf_, -1);
        }
        return Iterator(buf_ + length_ - 1, buf_, buf_ + length_, -1);
    }

    ConstIterator reverseIterator() const {
        if (length_ == 0) {
            return ConstIterator(nullptr, buf_, buf_, -1);
        }
        return ConstIterator(buf_ + length_ - 1, buf_, buf_ + length_, -1);
    }

    class Iterator {
    public:
        Iterator(T* p, T* begin, T* end, int step) :
            p_(p), begin_(begin), end_(end), step_(step) {
        }

        const T& get() const {
            return *p_;
        }
        void set(const T& value) {
            *p_ = value;
        }

        void next() {
            if (p_) {
                p_ += step_;
            }
        }
        bool hasNext() const {
            if (!p_) {
                return false;
            }

            if (step_ > 0) {
                return p_ < end_;
            }

            return p_ >= begin_;
        }

    private:
        T* p_;
        T* begin_;
        T* end_;
        int step_;
    };

    class ConstIterator {
    public:
        ConstIterator(const T* p,
            const T* begin, const T* end, int step) :
            p_(p), begin_(begin), end_(end), step_(step) {
        }

        const T& get() const {
            return *p_;
        }

        void next() {
            if (p_) {
                p_ += step_;
            }
        }

        bool hasNext() const {
            if (!p_) {
                return false;
            }
            if (step_ > 0) {
                return p_ < end_;
            }
            return p_ >= begin_;
        }

    private:
        const T* p_;
        const T* begin_;
        const T* end_;
        int step_;
    };

private:
    T* buf_;
    int length_;
    int capacity_;

    static constexpr int kDefCapacity_ = 8;

    void check_capacity() {
        if (capacity_ == 0) {
            capacity_ = kDefCapacity_;
        }

        if (!buf_) {
            create_buf((capacity_) * sizeof(T));
            length_ = 0;
            return;
        }

        if (length_ < capacity_) return;
        
        capacity_ *= 2;

        T* p = static_cast<T*>(malloc((capacity_) * sizeof(T)));

        if (!p) {
            throw std::bad_alloc();
        }

        for (int i = 0; i < length_; i++)
        {
            new (p + i) T(std::move(buf_[i]));
            buf_[i].~T();
        }

        free(buf_);
        buf_ = p;
    }

    void create_buf(size_t size) {
        buf_ = static_cast<T*>(malloc(size));
        if (!buf_) {
            throw std::bad_alloc();
        }
    }

    void delete_buf() {
        if (buf_) {
            for (int i = 0; i < length_; i++) {
                buf_[i].~T();
            }
            free(buf_);
        }
    }
    
};