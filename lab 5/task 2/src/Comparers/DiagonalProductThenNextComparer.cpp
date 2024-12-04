#include "Comparers/DiagonalProductThenNextComparer.h"

#include "Helpers.h"

int DiagonalProductThenNextComparer::Compare(
    const ArithmeticExpression& o1, const ArithmeticExpression& o2) const {
    Matrix m1 = o1.Evaluate();
    Matrix m2 = o2.Evaluate();

    double product1 = calculateDiagonalProduct(m1);
    double product2 = calculateDiagonalProduct(m2);

    if (product1 < product2) return -1;
    if (product1 > product2) return 1;

    return compareMatricesLex(m1, m2);
}
