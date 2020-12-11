cmake_minimum_required(VERSION 3.15)
project(recursive_descent_parser)

set(CMAKE_CXX_STANDARD 11)
add_compile_options(-Wall -Wextra -pedantic -Wfloat-equal -fno-stack-protector)


add_executable(recursive_descent_parser
        main.cpp
        src/math_tree.cpp
        src/math_tree.h)

add_executable(treeExpr
        src/treeExpr.cpp
        src/math_tree.cpp
        src/math_tree.h)

add_executable(eval
        src/math_tree.cpp
        src/math_tree.h
        src/eval.cpp)