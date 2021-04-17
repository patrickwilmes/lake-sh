set(SRC
        terminal.cpp
        terminal.hpp
)

add_library(term ${SRC})
target_link_libraries(term PRIVATE ncurses)
