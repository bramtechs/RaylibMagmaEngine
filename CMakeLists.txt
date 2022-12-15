cmake_minimum_required(VERSION 3.0)
project(magma C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wimplicit-function-declaration -Wbuiltin-declaration-mismatch -w")

# Adding Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

add_subdirectory(raylib)
add_subdirectory(raygui/projects/CMake)

# Declaring our executable
FILE(GLOB THESAUCE src/*.c src/*.h)
add_library(${PROJECT_NAME} STATIC ${THESAUCE})
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE} src)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib raygui)
