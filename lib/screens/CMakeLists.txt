cmake_minimum_required(VERSION 3.1)

project(screens C)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

find_package(Curses REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

add_library(ncurses_screen ncurses_screen.c)
target_link_libraries(ncurses_screen ${CURSES_LIBRARIES})

include_directories(${GTK4_INCLUDE_DIRS})
add_library(gtk_screen gtk_screen.c)
target_link_libraries(gtk_screen ${GTK4_LIBRARIES})