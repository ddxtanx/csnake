cmake_minimum_required(VERSION 3.1)

project(screens C)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

link_libraries(${CURSES_LIBRARIES} Threads::Threads)

add_library(ncurses_screen ncurses_screen.c)