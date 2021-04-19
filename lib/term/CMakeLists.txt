set(SRC
        Terminal.cpp
        Terminal.hpp

        TerminalObserver.cpp
        TerminalObserver.hpp
        )

add_library(term ${SRC})
target_link_libraries(term PRIVATE ncurses)
