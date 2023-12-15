cmake_minimum_required(VERSION 3.12)
project(AoCC)

set(CMAKE_C_STANDARD 99)

add_compile_options(-g -O0 -fsanitize=address -Wall -Wextra)
add_link_options(-fsanitize=address)
include_directories(include)

add_subdirectory(test)

set(day_list)

foreach(day RANGE 1 15)
    list(APPEND day_list src/day${day}.c)
endforeach()

add_library(aocc
    src/util.c
    src/re.c
    src/dynarr.c
    src/hashmap.c
    ${day_list}
)
target_link_libraries(aocc m)

add_executable(aocc_bin src/main.c)
target_link_libraries(aocc_bin aocc)
set_target_properties(aocc_bin PROPERTIES OUTPUT_NAME aocc)
