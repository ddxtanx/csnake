cmake_minimum_required(VERSION 3.1)

project(snake-lib C)

add_subdirectory(screens)

add_library(snake STATIC snake.c snake.h)