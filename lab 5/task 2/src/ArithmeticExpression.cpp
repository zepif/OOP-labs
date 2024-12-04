#include "ArithmeticExpression.h"

#include <algorithm>
#include <queue>
#include <stack>

#include "IComparer.h"
#include "MatrixException.h"

ArithmeticExpression::ArithmeticExpression() : root(nullptr), loader(nullptr) {}

ArithmeticExpression::ArithmeticExpression(
    ArithmeticExpression&& other) noexcept
    : root(std::move(other.root)), loader(std::move(other.loader)) {}

ArithmeticExpression& ArithmeticExpression::operator=(
    ArithmeticExpression&& other) noexcept {
    if (this != &other) {
        root = std::move(other.root);
        loader = std::move(other.loader);
    }
    return *this;
}

void ArithmeticExpression::setLoader(std::unique_ptr<Loader> newLoader) {
    loader = std::move(newLoader);
}

void ArithmeticExpression::addOperand(char oper) {
    if (!loader) {
        throw MatrixException("Loader not set");
    }

    Matrix operand = loader->GetItem();
    std::unique_ptr<Node> newOperand = std::make_unique<OperandNode>(operand);

    if (!root) {
        root = std::move(newOperand);
    } else if (oper != 0) {
        root = std::make_unique<OperatorNode>(oper, std::move(root),
                                              std::move(newOperand));
    } else {
        throw MatrixArithmeticException(
            "Operator not provided for operand addition");
    }
}

void ArithmeticExpression::switchLoader(std::unique_ptr<Loader> newLoader) {
    loader = std::move(newLoader);
}

ArithmeticExpression::Iterator::Iterator(Node* rootNode, size_t idx)
    : index(idx) {
    if (rootNode) {
        rootNode->collectOperands(operands);
    }
}

bool ArithmeticExpression::Iterator::operator!=(const Iterator& other) const {
    return index != other.index;
}

ArithmeticExpression& ArithmeticExpression::Iterator::operator*() const {
    throw MatrixArithmeticException(
        "Dereferencing Iterator is not implemented correctly.");
}

ArithmeticExpression* ArithmeticExpression::Iterator::operator->() const {
    throw MatrixArithmeticException(
        "Arrow Operator on Iterator is not implemented correctly.");
}

const ArithmeticExpression::Iterator&
ArithmeticExpression::Iterator::operator++() {
    ++index;
    return *this;
}

ArithmeticExpression::Iterator ArithmeticExpression::begin() {
    return Iterator(root.get(), 0);
}

ArithmeticExpression::Iterator ArithmeticExpression::end() {
    return Iterator(root.get(), getOperands().size());
}

std::string ArithmeticExpression::PrintExpression() const {
    if (!root) return "";
    return root->toString();
}

bool ArithmeticExpression::Find(const std::string& target) const {
    if (!root) return false;
    return root->find(target);
}

Matrix ArithmeticExpression::Evaluate() const {
    if (!root) {
        throw MatrixArithmeticException("Expression tree is empty");
    }

    std::unique_ptr<Node> evaluated = root->evaluate();
    OperandNode* resultNode = dynamic_cast<OperandNode*>(evaluated.get());
    if (resultNode) {
        return resultNode->getValue();
    } else {
        throw MatrixArithmeticException(
            "Evaluation did not result in an operand");
    }
}

bool ArithmeticExpression::StepEvaluate() {
    if (!root) return false;

    std::stack<Node*> stack;
    std::stack<Node*> output;

    stack.push(root.get());

    while (!stack.empty()) {
        Node* current = stack.top();
        stack.pop();
        output.push(current);

        if (current->isOperator()) {
            OperatorNode* opNode = dynamic_cast<OperatorNode*>(current);
            if (opNode) {
                stack.push(opNode->getLeft());
                stack.push(opNode->getRight());
            }
        }
    }

    while (!output.empty()) {
        Node* node = output.top();
        output.pop();

        if (node->isOperator()) {
            OperatorNode* opNode = dynamic_cast<OperatorNode*>(node);
            OperandNode* leftOperand =
                dynamic_cast<OperandNode*>(opNode->getLeft());
            OperandNode* rightOperand =
                dynamic_cast<OperandNode*>(opNode->getRight());

            if (leftOperand && rightOperand) {
                Matrix result;
                switch (opNode->getOperator()) {
                    case '+':
                        result =
                            leftOperand->getValue() + rightOperand->getValue();
                        break;
                    case '-':
                        result =
                            leftOperand->getValue() - rightOperand->getValue();
                        break;
                    case '*':
                        result =
                            leftOperand->getValue() * rightOperand->getValue();
                        break;
                    case '/':
                        result =
                            leftOperand->getValue() / rightOperand->getValue();
                        break;
                    default:
                        throw MatrixArithmeticException(
                            "Unknown operator during step evaluation");
                }

                replaceNode(node, std::make_unique<OperandNode>(result));

                return true;
            }
        }
    }

    return false;
}

void ArithmeticExpression::sort(
    const IComparer<ArithmeticExpression>& comparer) {
    throw MatrixArithmeticException(
        "Sort method is not applicable to a single ArithmeticExpression.");
}

void ArithmeticExpression::replaceNode(Node* target,
                                       std::unique_ptr<Node> replacement) {
    if (root.get() == target) {
        root = std::move(replacement);
        return;
    }

    std::queue<Node*> q;
    q.push(root.get());

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        if (current->isOperator()) {
            OperatorNode* opNode = dynamic_cast<OperatorNode*>(current);
            if (opNode->getLeft() == target) {
                opNode->getLeftPtr() = std::move(replacement);
                return;
            }
            if (opNode->getRight() == target) {
                opNode->getRightPtr() = std::move(replacement);
                return;
            }
            q.push(opNode->getLeft());
            q.push(opNode->getRight());
        }
    }
}

std::vector<Matrix*> ArithmeticExpression::getOperands() const {
    std::vector<Matrix*> operandsVec;
    if (root) {
        root->collectOperands(operandsVec);
    }
    return operandsVec;
}

