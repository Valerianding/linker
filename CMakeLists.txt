cmake_minimum_required(VERSION 3.16)
project(linker)

set(CMAKE_CXX_STANDARD 17)


include_directories(GLOBAL_RECURSE ./src)

file(GLOB_RECURSE HEADERS src/*.h LIST_DIRECTORIES true)
file(GLOB_RECURSE SOURCES "src/*.cpp" LIST_DIRECTORIES true)


MESSAGE(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
MESSAGE(STATUS "CMAKE_CUR_BINARY_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


add_executable(linker main.cpp ${SOURCES} )


