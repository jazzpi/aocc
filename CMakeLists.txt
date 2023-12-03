cmake_minimum_required(VERSION 3.12)
project(AoCC)

set(CMAKE_C_STANDARD 99)

add_compile_options(-g -Og -fsanitize=address -Wall -Wextra)
add_link_options(-fsanitize=address)
include_directories(include)

add_library(aocutil src/util.c)
link_libraries(aocutil)

add_executable(day1 src/day1.c)
add_executable(day2 src/day2.c)
add_executable(day3 src/day3.c)
