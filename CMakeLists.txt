cmake_minimum_required(VERSION 3.15)

if(DEFINED ${ONLY_LIBRARY})
    add_subdirectory(src)
else()
    project(CommandParser CXX C)
    set (CMAKE_CXX_STANDARD 17)

    add_subdirectory(external)
    add_subdirectory(src)

    add_executable(CommandParser_unitTests TEST_Command.cpp TEST_CommandManager.cpp TEST_Package.cpp TEST_Parser.cpp TEST_Packge&CommandManager.cpp)

    target_link_libraries(CommandParser_unitTests
            CommandParser
            gtest
            gmock
            gtest_main
            )

    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror=unused-parameter")
endif()
