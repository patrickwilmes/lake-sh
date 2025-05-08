set(SRC
        Parser.cpp
        Parser.hpp

        fs/WorkingDirectory.cpp
        fs/WorkingDirectory.hpp

        Shell.cpp
        Shell.hpp

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

        Executor.cpp
        Executor.hpp

        Prompt.hpp
        Prompt.cpp
        )

# We use the WSL_COMPATIBILITY_MODE to ensure that lake-sh is runnable in WSL 2
# For some reason, getting the username and also the current implementation of the
# path indexing is not working in wsl, but only on mac and linux (native).
if (EnableWslCompatibility)
    add_compile_definitions(WSL_COMPATIBILITY_MODE)
endif ()

add_executable(lsh main.cpp ${SRC})
target_link_libraries(lsh readline AK LibConfig)

add_executable(lsh_debug main.cpp ${SRC})
target_link_libraries(lsh_debug readline)
target_compile_definitions(lsh_debug PRIVATE SHELL_DEBUG)
