cmake_minimum_required(VERSION 3.20)
project(Snake_2 C)

set(CMAKE_C_STANDARD 99)

add_executable(Snake_2 main.c printLabirint.c boardOperations.h createBoard.c createLabirint.c randomWalls.c moving.c aStarAlgorithm.c aIAlgorithms.h randomAlgorithm.c)
