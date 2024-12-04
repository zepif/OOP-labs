#ifndef LOADER_H
#define LOADER_H

#include "Matrix.h"
#include <memory>
#include <string>

class Loader {
public:
    virtual ~Loader() = default;
    virtual Matrix GetItem() = 0;
};

class ConsoleLoader : public Loader {
public:
    virtual Matrix GetItem() override;
};

class FileLoader : public Loader {
private:
    std::string filename;

public:
    explicit FileLoader(const std::string& fname);
    virtual Matrix GetItem() override;
};

#endif // LOADER_H
