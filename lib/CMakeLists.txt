cmake_minimum_required(VERSION 3.1)

project(snake-lib C)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

add_subdirectory(screens)

add_library(snake snake.c snake.h)