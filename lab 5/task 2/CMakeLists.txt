cmake_minimum_required(VERSION 3.10)
project(ArithmeticExpression)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    src/Matrix.cpp
    src/Loader.cpp
    src/Node.cpp
    src/ArithmeticExpression.cpp
    src/VectorAnalog.cpp
    src/Helpers.cpp
    src/Comparers/DiagonalProductComparer.cpp
    src/Comparers/DiagonalProductThenNextComparer.cpp
    src/main.cpp
)

add_executable(ArithmeticExpression ${SOURCES})

