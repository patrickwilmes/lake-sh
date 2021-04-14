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

        utils/utils.cpp
        utils/utils.hpp

        history.cpp
        history.hpp

        shell_context.cpp
        shell_context.hpp

        )

set(CLION_SRC
        clion_launch.cpp
        )

add_executable(lsh main.cpp ${SRC} )
target_link_libraries(lsh ncurses)

add_executable(clion_debug ${CLION_SRC} ${SRC})
target_link_libraries(clion_debug ncurses)
