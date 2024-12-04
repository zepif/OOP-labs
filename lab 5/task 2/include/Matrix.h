#ifndef MATRIX_H
#define MATRIX_H

#include "MatrixException.h"
#include <string>

class Matrix {
private:
    double** data;
    size_t rows;
    size_t cols;
    
    void allocateMemory();
    void deallocateMemory();
    void copyData(const Matrix& other);

    double sum() const;
    bool willOverflow(double a, double b) const;

public:
    Matrix();
    Matrix(size_t r, size_t c);
    Matrix(double** arr, size_t r, size_t c);
    Matrix(double num);
    Matrix(const char* str);
    Matrix(const Matrix& other);
    Matrix(Matrix&& other) noexcept;
    ~Matrix();

    Matrix& operator=(const Matrix& other);
    Matrix& operator=(Matrix&& other) noexcept;
    
    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator/(const Matrix& other) const;

    Matrix operator+(const char* str) const;
    friend Matrix operator+(const char* str, const Matrix& matrix);

    Matrix operator-(const char* str) const;
    friend Matrix operator-(const char* str, const Matrix& matrix);

    Matrix operator*(const char* str) const;
    friend Matrix operator*(const char* str, const Matrix& matrix);

    Matrix operator/(const char* str) const;
    friend Matrix operator/(const char* str, const Matrix& matrix);

    Matrix& operator+=(const Matrix& other);
    Matrix& operator-=(const Matrix& other);
    Matrix& operator*=(const Matrix& other);
    Matrix& operator/=(const Matrix& other);
    
    bool operator==(const Matrix& other) const;
    bool operator!=(const Matrix& other) const;
    bool operator<(const Matrix& other) const;
    bool operator>(const Matrix& other) const;
    bool operator<=(const Matrix& other) const;
    bool operator>=(const Matrix& other) const;
    
    std::string toString() const;

    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;

    size_t getRows() const;
    size_t getCols() const;
};

#endif // MATRIX_H
