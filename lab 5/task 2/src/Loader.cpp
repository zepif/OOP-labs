#include "Loader.h"
#include "MatrixException.h"
#include <iostream>
#include <fstream>

Matrix ConsoleLoader::GetItem() {
    std::cout << "Enter matrix in format [a,b;c,d]: ";
    std::string input;
    std::cin >> input;
    return Matrix(input.c_str());
}

FileLoader::FileLoader(const std::string& fname) : filename(fname) {}

Matrix FileLoader::GetItem() {
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        throw MatrixException("Unable to open file: " + filename);
    }

    std::string input;
    std::getline(infile, input);
    infile.close();

    return Matrix(input.c_str());
}
