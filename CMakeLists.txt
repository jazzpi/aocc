cmake_minimum_required(VERSION 3.12)
project(AoCC)

set(CMAKE_C_STANDARD 99)

add_compile_options(-g -O0 -fsanitize=address -Wall -Wextra)
add_link_options(-fsanitize=address)
include_directories(include)

add_subdirectory(test)

add_library(aocc
    src/util.c
    src/re.c
    src/dynarr.c
    src/day1.c
    src/day2.c
    src/day3.c
    src/day4.c
)

add_executable(aocc_bin src/main.c)
target_link_libraries(aocc_bin aocc)
set_target_properties(aocc_bin PROPERTIES OUTPUT_NAME aocc)
