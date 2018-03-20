cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(tesselated_teapot LANGUAGES CXX)

add_subdirectory(src/libs/eigen)
add_subdirectory(src/libs/glfw-3.2.1)
add_subdirectory(src/libs/tl)
add_subdirectory(src/tesselated_teapot)
