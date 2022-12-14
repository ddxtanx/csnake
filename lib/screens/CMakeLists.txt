cmake_minimum_required(VERSION 3.1)

project(screens C)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

find_package(Curses REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(raylib 3.0 REQUIRED)

add_library(ncurses_screen ncurses_screen.c)
target_link_libraries(ncurses_screen ${CURSES_LIBRARIES})

add_library(gui_screen gui_screen.c)
target_link_libraries(gui_screen raylib)

if (APPLE)
    target_link_libraries(gui_screen "-framework IOKit")
    target_link_libraries(gui_screen "-framework Cocoa")
    target_link_libraries(gui_screen "-framework OpenGL")
endif()