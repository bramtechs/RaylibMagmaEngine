cmake_minimum_required(VERSION 3.0)
project(magmalib C)
set(CMAKE_C_STANDARD 11)

# Declaring our executable
FILE(GLOB THESAUCE *.c *.h)
add_library(${PROJECT_NAME} SHARED ${THESAUCE})

target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PUBLIC raylib raygui)
