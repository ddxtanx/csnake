cmake_minimum_required(VERSION 3.1)
project(Snake C)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
add_subdirectory(lib)

find_package(Threads REQUIRED)

add_executable(main main.c)
target_link_libraries(main snake ncurses_screen Threads::Threads)
