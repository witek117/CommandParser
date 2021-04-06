add_library(CommandParser STATIC
        arguments_parser.cpp
)

target_include_directories(CommandParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})