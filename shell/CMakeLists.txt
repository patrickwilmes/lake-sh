set(SRC
        lsh.cpp
        lsh.hpp

        tokenizer.cpp
        tokenizer.hpp

        usr.cpp
        usr.hpp

        cmd_assembler.cpp
        cmd_assembler.hpp

        cmd_handler.cpp
        cmd_handler.hpp

        cmd/cmd.hpp
        cmd/cmd_pwd.cpp
        cmd/cmd_cd.cpp

        main.cpp
        )

add_executable(lsh ${SRC} )
target_include_directories(lsh PRIVATE src)
target_link_libraries(lsh ncurses)
