#ifndef NODE_H
#define NODE_H

#include "Matrix.h"
#include "MatrixException.h"
#include <string>
#include <memory>
#include <vector>

class Node;

class Node {
public:
    virtual ~Node() = default;

    virtual bool isOperator() const = 0;

    virtual std::string toString() const = 0;

    virtual std::unique_ptr<Node> evaluate() = 0;

    virtual bool find(const std::string& target) const = 0;

    virtual void collectOperands(std::vector<Matrix*>& operands) = 0;
};

class OperandNode : public Node {
private:
    Matrix value;

public:
    explicit OperandNode(const Matrix& val);

    bool isOperator() const override;

    std::string toString() const override;

    std::unique_ptr<Node> evaluate() override;

    bool find(const std::string& target) const override;

    void collectOperands(std::vector<Matrix*>& operandsVec) override;

    const Matrix& getValue() const;
};

class OperatorNode : public Node {
private:
    char op; // '+', '-', '*', '/'
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

public:
    OperatorNode(char oper, std::unique_ptr<Node> lhs, std::unique_ptr<Node> rhs);

    bool isOperator() const override;

    std::string toString() const override;

    std::unique_ptr<Node> evaluate() override;

    bool find(const std::string& target) const override;

    void collectOperands(std::vector<Matrix*>& operandsVec) override;

    char getOperator() const;

    Node* getLeft() const;
    Node* getRight() const;

    std::unique_ptr<Node>& getLeftPtr();
    std::unique_ptr<Node>& getRightPtr();
};

#endif // NODE_H
