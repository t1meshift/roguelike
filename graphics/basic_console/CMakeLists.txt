set(GRAPHICS_MODULE_NAME basic_console)

cmake_minimum_required(VERSION 3.13)
project(graphics_${GRAPHICS_MODULE_NAME})

add_library(graphics_${GRAPHICS_MODULE_NAME} STATIC
        basic_console.h
        basic_console.cpp)