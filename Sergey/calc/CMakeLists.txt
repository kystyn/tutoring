project(calc LANGUAGES C)
cmake_minimum_required(VERSION 3.8)

add_executable(calc
    calc.c scanner.c scanner.h token.h utils.c utils.h error.h evaluator.c evaluator.h stack.c stack.h
    infix2polish.c infix2polish.h)
target_link_libraries(calc m)
