cmake_minimum_required(VERSION 3.31)
project(untitled4 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled4 main.c utils.c
        graph.h
        graph.c
        tarjan.h
        tarjan.c
        hasse.h
        hasse.c
        matrix.h
        matrix.c
)
