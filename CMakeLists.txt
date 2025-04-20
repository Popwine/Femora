cmake_minimum_required(VERSION 3.10)

project(fem_sim)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Wpedantic)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(mathcore STATIC ${PROJECT_SOURCE_DIR}/src/vector2d.cpp)

target_include_directories(mathcore PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(fem_sim ${SRC_FILES})

