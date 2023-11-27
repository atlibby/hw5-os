cmake_minimum_required(VERSION 3.23)
project(hw5_os C)

set(CMAKE_C_STANDARD 11)

add_executable(hw5_os scheduler-tests.c
                pqueue.h)
