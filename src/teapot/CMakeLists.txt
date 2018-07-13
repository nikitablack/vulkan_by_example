cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

find_package(Vulkan REQUIRED)

add_executable(tesselated_teapot "")

target_sources(tesselated_teapot PRIVATE
        src/app/App.h
        src/app/AppData.h
        src/app/create_window.cpp
        src/app/framebuffer_size_callback.cpp
        src/app/on_key_press.cpp

        src/main.cpp
        src/MainApplication.cpp
        src/MainApplication.h
        src/TeapotData.h)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CXXCOMPILEOPTIONS -Wall -ansi -pedantic -Wextra -Wshadow -Weffc++)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    string(REGEX REPLACE " /W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CXXCOMPILEOPTIONS /W4)
endif()

set_target_properties(tesselated_teapot PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_compile_options(tesselated_teapot PRIVATE ${CXXCOMPILEOPTIONS})

target_link_libraries(tesselated_teapot PRIVATE
        eigen
        glfw
        tl
        ${Vulkan_LIBRARIES}
)

target_include_directories(tesselated_teapot PRIVATE src)
target_include_directories(tesselated_teapot SYSTEM PRIVATE ${Vulkan_INCLUDE_DIRS})