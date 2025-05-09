set(SRC
        Token.cpp
        Token.h
        Tokenizer.cpp
        Tokenizer.h
        Parser.cpp
        Parser.h
        Repl.cpp
        Repl.h
        Command.cpp
        Command.h
)

add_library(LibRepl ${SRC})
target_include_directories(LibRepl PUBLIC .)

target_link_libraries(LibRepl PRIVATE AK)
