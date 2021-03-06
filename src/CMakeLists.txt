add_library(CommandParser STATIC
        argumets_parser.cpp
        command_manager.cpp
)

target_include_directories(CommandParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})