cmake_minimum_required(VERSION 3.0)
project(magma C)
set(CMAKE_C_STANDARD 11)

# Adding Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

add_subdirectory(raylib)
add_subdirectory(raygui/projects/CMake)
add_subdirectory(src)
