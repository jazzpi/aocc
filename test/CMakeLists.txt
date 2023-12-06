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

add_executable(test_util util.cpp)
add_executable(test_day1 day1.cpp)
add_executable(test_day2 day2.cpp)
add_executable(test_day3 day3.cpp)
add_executable(test_day4 day4.cpp)
add_executable(test_day5 day5.cpp)
add_executable(test_day6 day6.cpp)

include(GoogleTest)
gtest_discover_tests(test_util)
gtest_discover_tests(test_day1)
gtest_discover_tests(test_day2)
gtest_discover_tests(test_day3)
gtest_discover_tests(test_day4)
gtest_discover_tests(test_day5)
gtest_discover_tests(test_day6)
