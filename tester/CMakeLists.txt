cmake_minimum_required(VERSION 3.0)
project(tester C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Declaring our executable
FILE(GLOB TESTSAUCE src/*.cpp src/*.h)
add_executable(${PROJECT_NAME} ${TESTSAUCE})

target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_link_libraries(${PROJECT_NAME} PUBLIC magma)

