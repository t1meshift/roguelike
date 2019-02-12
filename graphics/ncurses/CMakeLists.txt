set(GRAPHICS_MODULE_NAME ncurses)

cmake_minimum_required(VERSION 3.13)
project(graphics_${GRAPHICS_MODULE_NAME})

add_library(graphics_${GRAPHICS_MODULE_NAME} STATIC
        ncurses.h
        ncurses.cpp)

target_link_libraries(graphics_${GRAPHICS_MODULE_NAME} ncurses)