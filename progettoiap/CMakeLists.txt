cmake_minimum_required(VERSION 3.20)
project(progettoiap C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(progettoiap
        dvector.c
        dvector.h
        main.c)
