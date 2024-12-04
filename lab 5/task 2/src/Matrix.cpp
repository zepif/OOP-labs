#include "Matrix.h"

#include <cfloat>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

void Matrix::allocateMemory() {
    try {
        data = new double*[rows];
        for (size_t i = 0; i < rows; ++i) {
            data[i] = new double[cols];
        }
    } catch (const std::bad_alloc&) {
        throw MatrixException(
            "Memory allocation failed during matrix initialization");
    }
}

void Matrix::deallocateMemory() {
    if (data) {
        for (size_t i = 0; i < rows; ++i) {
            delete[] data[i];
        }
        delete[] data;
    }
}

void Matrix::copyData(const Matrix& other) {
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

double Matrix::sum() const {
    double result = 0;
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            result += data[i][j];
        }
    }
    return result;
}

bool Matrix::willOverflow(double a, double b) const {
    return (b > 0 && a > DBL_MAX - b) || (b < 0 && a < -DBL_MAX - b);
}

Matrix::Matrix() : data(nullptr), rows(0), cols(0) {}

Matrix::Matrix(size_t r, size_t c) : rows(r), cols(c) {
    allocateMemory();
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = 0.0;
        }
    }
}

Matrix::Matrix(double** arr, size_t r, size_t c) : rows(r), cols(c) {
    allocateMemory();
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            data[i][j] = arr[i][j];
        }
    }
}

Matrix::Matrix(double num) : rows(1), cols(1) {
    allocateMemory();
    data[0][0] = num;
}

Matrix::Matrix(const char* str) {
    std::string input(str);
    if (input.empty() || input.front() != '[' || input.back() != ']') {
        throw InvalidMatrixFormatException(
            "Matrix must start with '[' and end with ']'");
    }

    input = input.substr(1, input.length() - 2);

    size_t rowCount = 1;
    size_t colCount = 1;
    for (char c : input) {
        if (c == ';') rowCount++;
        if (rowCount == 1 && c == ',') colCount++;
    }

    rows = rowCount;
    cols = colCount;
    allocateMemory();

    std::stringstream ss(input);
    std::string row;
    size_t i = 0;

    while (std::getline(ss, row, ';')) {
        std::stringstream rowStream(row);
        std::string value;
        size_t j = 0;

        while (std::getline(rowStream, value, ',')) {
            try {
                data[i][j] = std::stod(value);
            } catch (const std::invalid_argument&) {
                throw InvalidMatrixFormatException(
                    "Non-numeric value encountered");
            } catch (const std::out_of_range&) {
                throw MatrixOverflowException("Value out of range");
            }
            j++;
        }
        if (j != cols) {
            throw InvalidMatrixFormatException(
                "Inconsistent number of columns");
        }
        i++;
    }
}

Matrix::Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
    allocateMemory();
    copyData(other);
}

Matrix::Matrix(Matrix&& other) noexcept
    : data(other.data), rows(other.rows), cols(other.cols) {
    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;
}

Matrix::~Matrix() { deallocateMemory(); }

Matrix& Matrix::operator=(const Matrix& other) {
    if (this != &other) {
        deallocateMemory();
        rows = other.rows;
        cols = other.cols;
        allocateMemory();
        copyData(other);
    }
    return *this;
}

Matrix& Matrix::operator=(Matrix&& other) noexcept {
    if (this != &other) {
        deallocateMemory();
        data = other.data;
        rows = other.rows;
        cols = other.cols;
        other.data = nullptr;
        other.rows = 0;
        other.cols = 0;
    }
    return *this;
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw MatrixDimensionMismatchException(
            "Cannot add matrices of different dimensions");
    }

    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (willOverflow(data[i][j], other.data[i][j])) {
                throw MatrixOverflowException("Addition overflow");
            }
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw MatrixDimensionMismatchException(
            "Cannot subtract matrices of different dimensions");
    }

    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (willOverflow(data[i][j], -other.data[i][j])) {
                throw MatrixOverflowException("Subtraction overflow");
            }
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw MatrixDimensionMismatchException(
            "Cannot multiply matrices of different dimensions");
    }

    Matrix result(rows, other.cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < other.cols; ++j) {
            double sum = 0;
            for (size_t k = 0; k < cols; ++k) {
                if (data[i][k] != 0 && other.data[k][j] != 0) {
                    if (std::abs(data[i][k]) >
                        DBL_MAX / std::abs(other.data[k][j])) {
                        throw MatrixOverflowException(
                            "Multiplication overflow");
                    }
                }
                sum += data[i][k] * other.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }
    return result;
}

Matrix Matrix::operator/(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) {
        throw MatrixDimensionMismatchException(
            "Cannot divide matrices of different dimensions");
    }

    Matrix result(rows, cols);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (std::abs(other.data[i][j]) < 1e-10) {
                throw MatrixDivisionByZeroException(
                    "Division by zero in matrix element");
            } else {
                result.data[i][j] = data[i][j] / other.data[i][j];
            }
        }
    }
    return result;
}

Matrix Matrix::operator+(const char* str) const {
    Matrix other(str);
    return *this + other;
}

Matrix operator+(const char* str, const Matrix& matrix) {
    Matrix other(str);
    return other + matrix;
}

Matrix Matrix::operator-(const char* str) const {
    Matrix other(str);
    return *this - other;
}

Matrix operator-(const char* str, const Matrix& matrix) {
    Matrix other(str);
    return other - matrix;
}

Matrix Matrix::operator*(const char* str) const {
    Matrix other(str);
    return *this * other;
}

Matrix operator*(const char* str, const Matrix& matrix) {
    Matrix other(str);
    return other * matrix;
}

Matrix Matrix::operator/(const char* str) const {
    Matrix other(str);
    return *this / other;
}

Matrix operator/(const char* str, const Matrix& matrix) {
    Matrix other(str);
    return other / matrix;
}

Matrix& Matrix::operator+=(const Matrix& other) {
    *this = *this + other;
    return *this;
}

Matrix& Matrix::operator-=(const Matrix& other) {
    *this = *this - other;
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& other) {
    *this = *this * other;
    return *this;
}

Matrix& Matrix::operator/=(const Matrix& other) {
    *this = *this / other;
    return *this;
}

bool Matrix::operator==(const Matrix& other) const {
    if (rows != other.rows || cols != other.cols) return false;

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            if (data[i][j] != other.data[i][j]) return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& other) const { return !(*this == other); }

bool Matrix::operator<(const Matrix& other) const {
    return sum() < other.sum();
}

bool Matrix::operator>(const Matrix& other) const {
    return sum() > other.sum();
}

bool Matrix::operator<=(const Matrix& other) const {
    return sum() <= other.sum();
}

bool Matrix::operator>=(const Matrix& other) const {
    return sum() >= other.sum();
}

std::string Matrix::toString() const {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            ss << data[i][j];
            if (j < cols - 1) ss << ",";
        }
        if (i < rows - 1) ss << ";";
    }
    ss << "]";
    return ss.str();
}

double& Matrix::operator()(size_t row, size_t col) {
    if (row >= rows || col >= cols) {
        throw MatrixException("Index out of bounds");
    }
    return data[row][col];
}

const double& Matrix::operator()(size_t row, size_t col) const {
    if (row >= rows || col >= cols) {
        throw MatrixException("Index out of bounds");
    }
    return data[row][col];
}

size_t Matrix::getRows() const { return rows; }
size_t Matrix::getCols() const { return cols; }
