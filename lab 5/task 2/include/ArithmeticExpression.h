#ifndef ARITHMETIC_EXPRESSION_H
#define ARITHMETIC_EXPRESSION_H

#include <memory>
#include <string>
#include <vector>

#include "Loader.h"
#include "Node.h"

template <typename T>
class IComparer;

class ArithmeticExpression {
   private:
    std::unique_ptr<Node> root;
    std::unique_ptr<Loader> loader;

   public:
    ArithmeticExpression();
    ~ArithmeticExpression() = default;

    ArithmeticExpression(const ArithmeticExpression&) = delete;
    ArithmeticExpression& operator=(const ArithmeticExpression&) = delete;

    ArithmeticExpression(ArithmeticExpression&& other) noexcept;
    ArithmeticExpression& operator=(ArithmeticExpression&& other) noexcept;

    void setLoader(std::unique_ptr<Loader> newLoader);

    void addOperand(char oper = 0);

    void switchLoader(std::unique_ptr<Loader> newLoader);

    class Iterator {
       private:
        std::vector<Matrix*> operands;
        size_t index;

       public:
        Iterator(Node* rootNode, size_t idx = 0);

        bool operator!=(const Iterator& other) const;

        ArithmeticExpression& operator*() const;
        ArithmeticExpression* operator->() const;

        const Iterator& operator++();
    };

    Iterator begin();
    Iterator end();

    std::string PrintExpression() const;

    bool Find(const std::string& target) const;

    Matrix Evaluate() const;

    bool StepEvaluate();

    void sort(const IComparer<ArithmeticExpression>& comparer);

    std::vector<Matrix*> getOperands() const;

   private:
    void replaceNode(Node* target, std::unique_ptr<Node> replacement);

    // std::vector<Matrix*> getOperands() const;
};

#endif  // ARITHMETIC_EXPRESSION_H
