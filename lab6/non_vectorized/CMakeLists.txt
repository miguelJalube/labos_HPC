cmake_minimum_required(VERSION 3.12)
project(MyProgram)

set(CMAKE_C_STANDARD 11)

add_executable(MyProgram src/main.c)
add_subdirectory(powercap)
target_link_libraries(MyProgram powercap)