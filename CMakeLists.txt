cmake_minimum_required(VERSION 4.1)
project(Project2 C)

set(CMAKE_C_STANDARD 11)

add_executable(Project2 main.c
        Keypad.h
        Keypad.c
        CoreLogic/App.c
)
