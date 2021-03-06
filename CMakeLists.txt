cmake_minimum_required(VERSION 3.15)

project(CommandParser CXX C)
set (CMAKE_CXX_STANDARD 17)

add_subdirectory(external)
add_subdirectory(src)

add_executable(unit_tests main.cpp)
target_link_libraries(unit_tests
        CommandParser
        gtest
        gmock
        gtest_main
        )
