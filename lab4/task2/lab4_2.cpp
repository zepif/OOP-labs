#include <iostream>
#include <cstdlib>
#include <csignal>
#include <stdexcept>
#include "lab4_2.h"

void perform_cleanup() {
    std::cout << "Executing finally block: cleaning up resources.\n";
}

void signal_handler(int signal) {
    std::cout << "\nReceived signal: " << signal << "\n";
    std::exit(signal);
}

void terminate_handler() {
    std::cout << "Uncaught exception. Performing cleanup.\n";
    std::abort();
}

int main() {
    std::set_terminate(terminate_handler);

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);

    auto f = make_finally(perform_cleanup);
    
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
