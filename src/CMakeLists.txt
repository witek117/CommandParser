add_library(CommandParser STATIC
    CommandBase.cpp
    CommandSet.cpp
    PrintManager.cpp
    ItemBase.cpp
    Parser.cpp
)

target_include_directories(CommandParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(DEFINED ${ONLY_LIBRARY})
    target_include_directories(CommandParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../)
else()
    target_include_directories(CommandParser PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../SimpleStream)
endif()