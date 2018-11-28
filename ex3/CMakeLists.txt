cmake_minimum_required(VERSION 3.8)
project(ex3_final)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES SudokuTree.c SudokuSolver.c SudokuTree.h GenericDFS.h GenericDFS.c)
add_executable(ex3_final ${SOURCE_FILES})