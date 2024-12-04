#include <iostream>
#include <stdexcept>
#include "lab4_2.h"

int main() {
    try {
        std::cout << "Executing try block\n";

        auto f = make_finally([]() {
            std::cout << "Executing finally block\n";
        });

        throw std::runtime_error("An error has occurred");
    }
    catch (const std::exception& e) {
        std::cout << "Executing catch block: " << e.what() << "\n";
    }

    std::cout << "Program continues after exception handling\n";

    return 0;
}
