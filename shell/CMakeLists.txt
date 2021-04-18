set(SRC
        Shell.cpp
        Shell.hpp

        Tokenizer.cpp
        Tokenizer.hpp

        User.cpp
        User.hpp

        CmdAssembler.cpp
        CmdAssembler.hpp

        CmdHandler.cpp
        CmdHandler.hpp

        utils/Utils.cpp
        utils/Utils.hpp

        History.cpp
        History.hpp

        ShellContext.cpp
        ShellContext.hpp

        Alias.cpp
        Alias.hpp

        Cmd.hpp
        Cmd.cpp

        Logger.cpp
        Logger.hpp

        Executor.cpp
        Executor.hpp
        )

add_executable(lsh main.cpp ${SRC} )
target_link_libraries(lsh ncurses term)
target_include_directories(lsh PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/term
        )
