#include "VectorAnalog.h"

#include <algorithm>
#include <utility>

#include "MatrixException.h"

const size_t INITIAL_CAPACITY = 4;

VectorAnalog::VectorAnalog() : data(nullptr), capacity(0), size_(0) {
    capacity = INITIAL_CAPACITY;
    data = std::make_unique<ArithmeticExpression[]>(capacity);
}

VectorAnalog::VectorAnalog(VectorAnalog&& other) noexcept
    : data(std::move(other.data)),
      capacity(other.capacity),
      size_(other.size_) {
    other.capacity = 0;
    other.size_ = 0;
}

VectorAnalog& VectorAnalog::operator=(VectorAnalog&& other) noexcept {
    if (this != &other) {
        data = std::move(other.data);
        capacity = other.capacity;
        size_ = other.size_;
        other.capacity = 0;
        other.size_ = 0;
    }
    return *this;
}

void VectorAnalog::resize(size_t new_capacity) {
    auto new_data = std::make_unique<ArithmeticExpression[]>(new_capacity);
    for (size_t i = 0; i < size_; ++i) {
        new_data[i] = std::move(data[i]);
    }
    data = std::move(new_data);
    capacity = new_capacity;
}

void VectorAnalog::add(ArithmeticExpression&& expr) {
    if (size_ >= capacity) {
        resize(capacity * 2);
    }
    data[size_] = std::move(expr);
    size_++;
}

void VectorAnalog::remove(size_t index) {
    if (index >= size_) {
        throw MatrixException("Index out of bounds in VectorAnalog::remove");
    }
    for (size_t i = index; i < size_ - 1; ++i) {
        data[i] = std::move(data[i + 1]);
    }
    size_--;
}

ArithmeticExpression& VectorAnalog::operator[](size_t index) {
    if (index >= size_) {
        throw MatrixException(
            "Index out of bounds in VectorAnalog::operator[]");
    }
    return data[index];
}

const ArithmeticExpression& VectorAnalog::operator[](size_t index) const {
    if (index >= size_) {
        throw MatrixException(
            "Index out of bounds in VectorAnalog::operator[]");
    }
    return data[index];
}

size_t VectorAnalog::size() const { return size_; }

VectorAnalog::Iterator VectorAnalog::begin() { return Iterator(data.get()); }

VectorAnalog::Iterator VectorAnalog::end() {
    return Iterator(data.get() + size_);
}

void VectorAnalog::printAll() const {
    for (size_t i = 0; i < size_; ++i) {
        std::cout << "Expression " << i << ": " << data[i].PrintExpression()
                  << std::endl;
    }
}

void VectorAnalog::sort(const IComparer<ArithmeticExpression>& comparer) {
    std::sort(data.get(), data.get() + size_,
              [&](const ArithmeticExpression& a, const ArithmeticExpression& b)
                  -> bool { return comparer.Compare(a, b) < 0; });
}
