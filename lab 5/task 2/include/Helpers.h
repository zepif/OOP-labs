#ifndef HELPERS_H
#define HELPERS_H

#include "Matrix.h"
#include "MatrixException.h"

double calculateDiagonalProduct(const Matrix& matrix);

int compareMatricesLex(const Matrix& m1, const Matrix& m2);

#endif  // HELPERS_H
