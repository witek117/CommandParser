cmake_minimum_required(VERSION 3.15)

if(DEFINED ${ONLY_LIBRARY})
    add_subdirectory(src)
else()
    project(CommandParser CXX C)
    set (CMAKE_CXX_STANDARD 17)

    add_subdirectory(external)
    add_subdirectory(src)

    add_executable(unit_tests TEST_CommandManager.cpp TEST_Package.cpp TEST_Command.cpp TEST_Packge&CommandManager.cpp)
    target_link_libraries(unit_tests
            CommandParser
            gtest
            gmock
            gtest_main
            )
endif()
