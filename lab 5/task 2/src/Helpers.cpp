#include "Helpers.h"

#include <algorithm>

double calculateDiagonalProduct(const Matrix& matrix) {
    size_t rows = matrix.getRows();
    size_t cols = matrix.getCols();
    if (rows != cols) {
        throw MatrixArithmeticException(
            "Matrix is not square for diagonal product calculation");
    }

    double mainDiagonalSum = 0.0;
    double secondaryDiagonalSum = 0.0;
    for (size_t i = 0; i < rows; ++i) {
        mainDiagonalSum += matrix(i, i);
        secondaryDiagonalSum += matrix(i, rows - i - 1);
    }

    return mainDiagonalSum * secondaryDiagonalSum;
}

int compareMatricesLex(const Matrix& m1, const Matrix& m2) {
    size_t minRows = std::min(m1.getRows(), m2.getRows());
    size_t minCols = std::min(m1.getCols(), m2.getCols());

    for (size_t i = 0; i < minRows; ++i) {
        for (size_t j = 0; j < minCols; ++j) {
            if (m1(i, j) < m2(i, j)) return -1;
            if (m1(i, j) > m2(i, j)) return 1;
        }
    }

    if (m1.getRows() < m2.getRows()) return -1;
    if (m1.getRows() > m2.getRows()) return 1;
    if (m1.getCols() < m2.getCols()) return -1;
    if (m1.getCols() > m2.getCols()) return 1;

    return 0;
}
