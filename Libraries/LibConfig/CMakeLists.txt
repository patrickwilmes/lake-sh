set(SRC
        TomlLexer.cpp
        TomlLexer.h
        Token.cpp
        Token.h)

add_library(LibConfig ${SRC})
target_include_directories(LibConfig PUBLIC .)

target_link_libraries(LibConfig PRIVATE AK)