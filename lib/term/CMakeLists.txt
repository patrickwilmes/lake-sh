set(SRC
        Terminal.cpp
        Terminal.hpp
)

add_library(term ${SRC})
target_link_libraries(term PRIVATE ncurses)
