#ifndef DIAGONAL_PRODUCT_COMPARER_H
#define DIAGONAL_PRODUCT_COMPARER_H

#include <stdexcept>

#include "../ArithmeticExpression.h"
#include "../IComparer.h"

class DiagonalProductComparer : public IComparer<ArithmeticExpression> {
   public:
    virtual int Compare(const ArithmeticExpression& o1,
                        const ArithmeticExpression& o2) const override;
};

#endif  // DIAGONAL_PRODUCT_COMPARER_H
