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

        utils/utils.cpp
        utils/utils.hpp

        history.cpp
        history.hpp

        shell_context.cpp
        shell_context.hpp

        alias.cpp
        alias.hpp

        cmd.hpp
        cmd.cpp

        logger.cpp
        logger.hpp

        executor.cpp
        executor.hpp
        )

add_executable(lsh main.cpp ${SRC} )
target_link_libraries(lsh ncurses term)
target_include_directories(lsh PRIVATE ../term)
