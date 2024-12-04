#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstring>
#include <cfloat>

class MatrixException : public std::exception {
protected:
    std::string message;

public:
    MatrixException(const std::string& msg) : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }

    virtual ~MatrixException() = default;
};

class InvalidMatrixFormatException : public MatrixException {
public:
    InvalidMatrixFormatException(const std::string& msg)
        : MatrixException("Invalid matrix format: " + msg) {}
};

class MatrixDimensionMismatchException : public MatrixException {
public:
    MatrixDimensionMismatchException(const std::string& msg)
        : MatrixException("Matrix dimension mismatch: " + msg) {}
};

class MatrixOverflowException : public MatrixException {
public:
    MatrixOverflowException(const std::string& msg)
        : MatrixException("Matrix overflow: " + msg) {}
};

class MatrixDivisionByZeroException : public MatrixException {
public:
    MatrixDivisionByZeroException(const std::string& msg)
        : MatrixException("Matrix division by zero: " + msg) {}
};

class MatrixArithmeticException : public MatrixException {
public:
    MatrixArithmeticException(const std::string& msg)
        : MatrixException("Matrix arithmetic error: " + msg) {}
};

class Matrix {
private:
    double** data;
    size_t rows;
    size_t cols;
    
    void allocateMemory() {
        try {
            data = new double*[rows];
            for (size_t i = 0; i < rows; ++i) {
                data[i] = new double[cols];
            }
        } catch (const std::bad_alloc&) {
            throw MatrixException("Memory allocation failed during matrix initialization");
        }
    }
    
    void deallocateMemory() {
        if (data) {
            for (size_t i = 0; i < rows; ++i) {
                delete[] data[i];
            }
            delete[] data;
        }
    }
    
    void copyData(const Matrix& other) {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = other.data[i][j];
            }
        }
    }

    double sum() const {
        double result = 0;
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result += data[i][j];
            }
        }
        return result;
    }
    
    bool willOverflow(double a, double b) const {
        return (b > 0 && a > DBL_MAX - b) || (b < 0 && a < -DBL_MAX - b);
    }

public:
    Matrix() : data(nullptr), rows(0), cols(0) {}
    
    Matrix(size_t r, size_t c) : rows(r), cols(c) {
        allocateMemory();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = 0.0;
            }
        }
    }
    
    Matrix(double** arr, size_t r, size_t c) : rows(r), cols(c) {
        allocateMemory();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                data[i][j] = arr[i][j];
            }
        }
    }
    
    Matrix(double num) : rows(1), cols(1) {
        allocateMemory();
        data[0][0] = num;
    }

    Matrix(const char* str) {
        std::string input(str);
        if (input.front() != '[' || input.back() != ']') {
            throw InvalidMatrixFormatException("Matrix must start with '[' and end with ']'");
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
                data[i][j] = std::stod(value);
                j++;
            }
            i++;
        }
    }

    Matrix(const Matrix& other) : rows(other.rows), cols(other.cols) {
        allocateMemory();
        copyData(other);
    }
    
    Matrix(Matrix&& other) noexcept : data(other.data), rows(other.rows), cols(other.cols) {
        other.data = nullptr;
        other.rows = 0;
        other.cols = 0;
    }
    
    ~Matrix() {
        deallocateMemory();
    }

    Matrix& operator=(const Matrix& other) {
        if (this != &other) {
            deallocateMemory();
            rows = other.rows;
            cols = other.cols;
            allocateMemory();
            copyData(other);
        }
        return *this;
    }

    Matrix& operator=(Matrix&& other) noexcept {
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
    
    Matrix operator+(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw MatrixDimensionMismatchException("Cannot add matrices of different dimensions");
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

    Matrix operator-(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw MatrixDimensionMismatchException("Cannot subtract matrices of different dimensions");
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

    Matrix operator*(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw MatrixDimensionMismatchException("Cannot multiply matrices of different dimensions");
        }
        
        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.cols; ++j) {
                double sum = 0;
                for (size_t k = 0; k < cols; ++k) {
                    if (data[i][k] != 0 && other.data[k][j] != 0) {
                        if (std::abs(data[i][k]) > DBL_MAX / std::abs(other.data[k][j])) {
                            throw MatrixOverflowException("Multiplication overflow");
                        }
                    }
                    sum += data[i][k] * other.data[k][j];
                }
                result.data[i][j] = sum;
            }
        }
        return result;
    }

    Matrix operator/(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw MatrixDimensionMismatchException("Cannot divide matrices of different dimensions");
        }
        
        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (std::abs(other.data[i][j]) < 1e-10) {
                    throw MatrixDivisionByZeroException("Division by zero in matrix element");
                } else {
                    result.data[i][j] = data[i][j] / other.data[i][j];
                }
            }
        }
        return result;
    }

    Matrix operator+(const char* str) const {
        Matrix other(str);
        return *this + other;
    }
    
    friend Matrix operator+(const char* str, const Matrix& matrix) {
        Matrix other(str);
        return other + matrix;
    }

    Matrix operator-(const char* str) const {
        Matrix other(str);
        return *this - other;
    }
    
    friend Matrix operator-(const char* str, const Matrix& matrix) {
        Matrix other(str);
        return other - matrix;
    }

    Matrix operator*(const char* str) const {
        Matrix other(str);
        return *this * other;
    }
    
    friend Matrix operator*(const char* str, const Matrix& matrix) {
        Matrix other(str);
        return other * matrix;
    }

    Matrix operator/(const char* str) const {
        Matrix other(str);
        return *this / other;
    }
    
    friend Matrix operator/(const char* str, const Matrix& matrix) {
        Matrix other(str);
        return other / matrix;
    }

    Matrix& operator+=(const Matrix& other) {
        *this = *this + other;
        return *this;
    }

    Matrix& operator-=(const Matrix& other) {
        *this = *this - other;
        return *this;
    }

    Matrix& operator*=(const Matrix& other) {
        *this = *this * other;
        return *this;
    }

    Matrix& operator/=(const Matrix& other) {
        *this = *this / other;
        return *this;
    }
    
    bool operator==(const Matrix& other) const {
        if (rows != other.rows || cols != other.cols) return false;
        
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                if (data[i][j] != other.data[i][j]) return false;
            }
        }
        return true;
    }
    
    bool operator!=(const Matrix& other) const {
        return !(*this == other);
    }
    
    bool operator<(const Matrix& other) const {
        return sum() < other.sum();
    }

    bool operator>(const Matrix& other) const {
        return sum() > other.sum();
    }

    bool operator<=(const Matrix& other) const {
        return sum() <= other.sum();
    }

    bool operator>=(const Matrix& other) const {
        return sum() >= other.sum();
    }
    
    std::string toString() const {
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
};

int main() {
    try {
        Matrix m1("[1,2,3;4,5,6;7,8,9]");
        Matrix m2("[2,0,1;1,2,1;0,1,2]");
        Matrix scalar(5.0);
        
        std::cout << "Matrix 1: " << m1.toString() << std::endl;
        std::cout << "Matrix 2: " << m2.toString() << std::endl;
        std::cout << "Scalar matrix: " << scalar.toString() << std::endl;

        std::cout << "\nMatrix addition:" << std::endl;
        Matrix sum = m1 + m2;
        std::cout << "M1 + M2 = " << sum.toString() << std::endl;

        std::cout << "\nMatrix subtraction:" << std::endl;
        Matrix diff = m1 - m2;
        std::cout << "M1 - M2 = " << diff.toString() << std::endl;

        std::cout << "\nMatrix multiplication:" << std::endl;
        Matrix prod = m1 * m2;
        std::cout << "M1 * M2 = " << prod.toString() << std::endl;

        Matrix m3 = m1;
        m3 += m2;
        std::cout << "\nCompound addition (M3 += M2): " << m3.toString() << std::endl;

        std::cout << "\nComparisons:" << std::endl;
        std::cout << "M1 == M2: " << (m1 == m2) << std::endl;
        std::cout << "M1 != M2: " << (m1 != m2) << std::endl;
        std::cout << "M1 < M2: " << (m1 < m2) << std::endl;
        std::cout << "M1 > M2: " << (m1 > m2) << std::endl;

        Matrix m4("[1,2;3,0]");
        Matrix m5("[2,1;1,0]");
        std::cout << "\nMatrix division:" << std::endl;
        Matrix div = m4 / m5;
        std::cout << "M4 / M5 = " << div.toString() << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
