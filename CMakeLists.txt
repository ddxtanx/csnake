cmake_minimum_required(VERSION 3.1)
project(Snake C)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
add_subdirectory(lib)

find_package(Curses REQUIRED)
find_package(Threads REQUIRED)

add_executable(snake_text main_ncurses.c)
target_link_libraries(snake_text ${CURSES_LIBRARIES} Threads::Threads snake ncurses_screen)
