#ifndef MATRIX_EXCEPTION_H
#define MATRIX_EXCEPTION_H

#include <exception>
#include <string>

class MatrixException : public std::exception {
protected:
    std::string message;

public:
    explicit MatrixException(const std::string& msg) : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }

    virtual ~MatrixException() = default;
};

class InvalidMatrixFormatException : public MatrixException {
public:
    explicit InvalidMatrixFormatException(const std::string& msg)
        : MatrixException("Invalid matrix format: " + msg) {}
};

class MatrixDimensionMismatchException : public MatrixException {
public:
    explicit MatrixDimensionMismatchException(const std::string& msg)
        : MatrixException("Matrix dimension mismatch: " + msg) {}
};

class MatrixOverflowException : public MatrixException {
public:
    explicit MatrixOverflowException(const std::string& msg)
        : MatrixException("Matrix overflow: " + msg) {}
};

class MatrixDivisionByZeroException : public MatrixException {
public:
    explicit MatrixDivisionByZeroException(const std::string& msg)
        : MatrixException("Matrix division by zero: " + msg) {}
};

class MatrixArithmeticException : public MatrixException {
public:
    explicit MatrixArithmeticException(const std::string& msg)
        : MatrixException("Matrix arithmetic error: " + msg) {}
};

#endif // MATRIX_EXCEPTION_H
