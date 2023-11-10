cmake_minimum_required(VERSION 3.26)
project(hw5_os C)

set(CMAKE_C_STANDARD 11)

add_executable(hw5_os scheduler-tests.c
        scheduler.atlibby.alynkirscht.c
        scheduler.atlibby.alynkirscht.h)
