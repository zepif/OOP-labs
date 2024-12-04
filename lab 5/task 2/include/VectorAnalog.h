#ifndef VECTOR_ANALOG_H
#define VECTOR_ANALOG_H

#include <iostream>
#include <memory>

#include "ArithmeticExpression.h"
#include "IComparer.h"

class VectorAnalog {
   private:
    std::unique_ptr<ArithmeticExpression[]> data;
    size_t capacity;
    size_t size_;

    void resize(size_t new_capacity);

   public:
    VectorAnalog();
    ~VectorAnalog() = default;

    VectorAnalog(const VectorAnalog&) = delete;
    VectorAnalog& operator=(const VectorAnalog&) = delete;

    VectorAnalog(VectorAnalog&& other) noexcept;
    VectorAnalog& operator=(VectorAnalog&& other) noexcept;

    void add(ArithmeticExpression&& expr);

    void remove(size_t index);

    ArithmeticExpression& operator[](size_t index);
    const ArithmeticExpression& operator[](size_t index) const;

    size_t size() const;

    class Iterator {
       private:
        ArithmeticExpression* current;

       public:
        Iterator(ArithmeticExpression* ptr) : current(ptr) {}

        ArithmeticExpression& operator*() const { return *current; }
        ArithmeticExpression* operator->() const { return current; }

        Iterator& operator++() {
            current++;
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    Iterator begin();
    Iterator end();

    void printAll() const;

    void sort(const IComparer<ArithmeticExpression>& comparer);
};

#endif  // VECTOR_ANALOG_H
