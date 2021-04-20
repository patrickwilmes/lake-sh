set(SRC
        Parser.cpp
        Parser.hpp

        Shell.cpp
        Shell.hpp

        User.cpp
        User.hpp

        CmdHandler.cpp
        CmdHandler.hpp

        utils/Utils.cpp
        utils/Utils.hpp

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

add_executable(lsh main.cpp ${SRC})
target_link_libraries(lsh readline)

add_executable(lsh_debug main.cpp ${SRC})
target_link_libraries(lsh_debug readline)
target_compile_definitions(lsh_debug PRIVATE SHELL_DEBUG)
