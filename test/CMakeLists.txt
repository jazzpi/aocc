# GoogleTest requires at leastC++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

link_libraries(GTest::gtest_main)
link_libraries(aocc)

add_library(aocc_testutil testutil.cpp)
link_libraries(aocc_testutil)

include(GoogleTest)

add_executable(test_util util.cpp)
add_executable(test_hashmap hashmap.cpp)
gtest_discover_tests(test_util)

foreach(day RANGE 1 11)
    add_executable(test_day${day} day${day}.cpp)
    gtest_discover_tests(test_day${day})
endforeach()
