cmake_minimum_required(VERSION 3.15)

if(DEFINED ${ONLY_LIBRARY})
    add_subdirectory(src)
else()
    project(CommandParser CXX C)
    set (CMAKE_CXX_STANDARD 17)

    add_subdirectory(external)
    add_subdirectory(src)

    add_executable(CommandParser_unitTests
            tests/TEST_Command.cpp
            tests/TEST_StreamCommandManager.cpp
#            tests/TEST_CommandManager.cpp
            tests/TEST_Package.cpp
            tests/TEST_Parser.cpp
            tests/TEST_Package&CommandManager.cpp
            )

    target_link_libraries(CommandParser_unitTests
            CommandParser
            gtest
            gmock
            gtest_main
            )

    SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Werror=unused-parameter")
endif()
