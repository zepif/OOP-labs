#include <iostream>
#include <memory>

#include "ArithmeticExpression.h"
#include "Comparers/DiagonalProductComparer.h"
#include "Comparers/DiagonalProductThenNextComparer.h"
#include "Loader.h"
#include "MatrixException.h"
#include "VectorAnalog.h"

int main() {
    try {
        VectorAnalog vector;

        std::unique_ptr<Loader> consoleLoader =
            std::make_unique<ConsoleLoader>();
        std::unique_ptr<Loader> fileLoader =
            std::make_unique<FileLoader>("matrix.txt");

        std::cout << "Крок а: Створення арифметичного виразу А з консолі."
                  << std::endl;
        ArithmeticExpression exprA;
        exprA.setLoader(std::make_unique<ConsoleLoader>());
        exprA.addOperand();
        exprA.addOperand('+');
        std::cout << "Вираз А: " << exprA.PrintExpression() << std::endl;

        std::cout
            << "\nКрок b: Обчислення виразу А з виведенням стану контейнера."
            << std::endl;
        Matrix resultA = exprA.Evaluate();
        std::cout << "Результат виразу А: " << resultA.toString() << std::endl;

        vector.add(std::move(exprA));
        vector.printAll();

        std::cout << "\nКрок b: Створення арифметичного виразу B з файлу."
                  << std::endl;
        ArithmeticExpression exprB;
        exprB.setLoader(std::make_unique<FileLoader>("matrix.txt"));
        exprB.addOperand();
        exprB.addOperand('*');
        std::cout << "Вираз B: " << exprB.PrintExpression() << std::endl;

        std::cout
            << "\nКрок c: Обчислення виразу B з виведенням стану контейнера."
            << std::endl;
        Matrix resultB = exprB.Evaluate();
        std::cout << "Результат виразу B: " << resultB.toString() << std::endl;

        vector.add(std::move(exprB));
        vector.printAll();

        std::cout << "\nКрок d: Додавання нової дії до виразу B через консоль."
                  << std::endl;
        ArithmeticExpression exprB_new;
        exprB_new.setLoader(std::make_unique<ConsoleLoader>());
        exprB_new.addOperand();
        exprB_new.addOperand('-');
        std::cout << "Новий вираз B: " << exprB_new.PrintExpression()
                  << std::endl;

        Matrix newResultB = exprB_new.Evaluate();
        std::cout << "Новий результат виразу B: " << newResultB.toString()
                  << std::endl;

        vector.add(std::move(exprB_new));
        vector.printAll();

        std::cout << "\nКрок e: Виведення елементів-математичних об'єктів "
                     "виразу B за допомогою ітератора."
                  << std::endl;
        if (vector.size() > 1) {
            ArithmeticExpression& exprB_ref = vector[1];
            std::cout << "Вираз B: " << exprB_ref.PrintExpression()
                      << std::endl;

            std::vector<Matrix*> operands = exprB_ref.getOperands();
            for (const auto& mat_ptr : operands) {
                std::cout << mat_ptr->toString() << std::endl;
            }
        } else {
            std::cerr << "Вираз B не знайдений у контейнері." << std::endl;
        }

        std::cout << "\nКрок f: Створення нового контейнера VectorAnalog та "
                     "додавання 4-5 ArithmeticExpression."
                  << std::endl;
        VectorAnalog vector2;
        for (int i = 1; i <= 5; ++i) {
            ArithmeticExpression expr;
            expr.setLoader(std::make_unique<ConsoleLoader>());
            std::cout << "Введіть матриці для виразу " << i << ":" << std::endl;
            expr.addOperand();
            char oper;
            std::cout << "Введіть оператор (+, -, *, /): ";
            std::cin >> oper;
            expr.addOperand(oper);
            vector2.add(std::move(expr));
        }
        vector2.printAll();

        std::cout << "\nКрок g: Сортування VectorAnalog за допомогою "
                     "DiagonalProductComparer та виведення результату."
                  << std::endl;
        DiagonalProductComparer comparer1;
        vector2.sort(comparer1);
        vector2.printAll();

        std::cout << "\nКрок h: Сортування VectorAnalog за допомогою "
                     "DiagonalProductThenNextComparer та виведення результату."
                  << std::endl;
        DiagonalProductThenNextComparer comparer2;
        vector2.sort(comparer2);
        vector2.printAll();

    } catch (const MatrixArithmeticException& mae) {
        std::cerr << "Матриця помилка: " << mae.what() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Загальна помилка: " << e.what() << std::endl;
    }

    return 0;
}
