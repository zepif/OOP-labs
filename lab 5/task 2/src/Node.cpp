#include "Node.h"
#include <stdexcept>

OperandNode::OperandNode(const Matrix& val) : value(val) {}

bool OperandNode::isOperator() const {
    return false;
}

std::string OperandNode::toString() const {
    return value.toString();
}

std::unique_ptr<Node> OperandNode::evaluate() {
    return std::make_unique<OperandNode>(value);
}

bool OperandNode::find(const std::string& target) const {
    return value.toString() == target;
}

void OperandNode::collectOperands(std::vector<Matrix*>& operandsVec) {
    operandsVec.push_back(const_cast<Matrix*>(&value));
}

const Matrix& OperandNode::getValue() const {
    return value;
}

OperatorNode::OperatorNode(char oper, std::unique_ptr<Node> lhs, std::unique_ptr<Node> rhs)
    : op(oper), left(std::move(lhs)), right(std::move(rhs)) {}

bool OperatorNode::isOperator() const {
    return true;
}

std::string OperatorNode::toString() const {
    return "(" + left->toString() + " " + std::string(1, op) + " " + right->toString() + ")";
}

std::unique_ptr<Node> OperatorNode::evaluate() {
    std::unique_ptr<Node> evaluatedLeft = left->evaluate();
    std::unique_ptr<Node> evaluatedRight = right->evaluate();

    if (!evaluatedLeft || !evaluatedRight) {
        throw MatrixArithmeticException("Invalid operands for operator " + std::string(1, op));
    }

    const OperandNode* leftOperand = dynamic_cast<OperandNode*>(evaluatedLeft.get());
    const OperandNode* rightOperand = dynamic_cast<OperandNode*>(evaluatedRight.get());

    if (!leftOperand || !rightOperand) {
        throw MatrixArithmeticException("Operands must be Matrix objects");
    }

    Matrix result;
    switch (op) {
        case '+':
            result = leftOperand->getValue() + rightOperand->getValue();
            break;
        case '-':
            result = leftOperand->getValue() - rightOperand->getValue();
            break;
        case '*':
            result = leftOperand->getValue() * rightOperand->getValue();
            break;
        case '/':
            result = leftOperand->getValue() / rightOperand->getValue();
            break;
        default:
            throw MatrixArithmeticException("Unknown operator");
    }

    return std::make_unique<OperandNode>(result);
}

bool OperatorNode::find(const std::string& target) const {
    return left->find(target) || right->find(target);
}

void OperatorNode::collectOperands(std::vector<Matrix*>& operandsVec) {
    left->collectOperands(operandsVec);
    right->collectOperands(operandsVec);
}

char OperatorNode::getOperator() const {
    return op;
}

Node* OperatorNode::getLeft() const {
    return left.get();
}

Node* OperatorNode::getRight() const {
    return right.get();
}

std::unique_ptr<Node>& OperatorNode::getLeftPtr() {
    return left;
}

std::unique_ptr<Node>& OperatorNode::getRightPtr() {
    return right;
}
