set(SRC
        catch.hpp

        ${PROJECT_SOURCE_DIR}/shell/Tokenizer.cpp
        ${PROJECT_SOURCE_DIR}/shell/Tokenizer.hpp

        ${PROJECT_SOURCE_DIR}/shell/CmdAssembler.cpp
        ${PROJECT_SOURCE_DIR}/shell/CmdAssembler.hpp

        ${PROJECT_SOURCE_DIR}/shell/Shell.cpp
        ${PROJECT_SOURCE_DIR}/shell/Shell.hpp

        ${PROJECT_SOURCE_DIR}/shell/User.cpp
        ${PROJECT_SOURCE_DIR}/shell/User.hpp

        ${PROJECT_SOURCE_DIR}/shell/CmdHandler.cpp
        ${PROJECT_SOURCE_DIR}/shell/CmdHandler.hpp

        ${PROJECT_SOURCE_DIR}/shell/utils/Utils.cpp
        ${PROJECT_SOURCE_DIR}/shell/utils/Utils.hpp

        ${PROJECT_SOURCE_DIR}/shell/History.cpp
        ${PROJECT_SOURCE_DIR}/shell/History.hpp

        ${PROJECT_SOURCE_DIR}/shell/ShellContext.cpp
        ${PROJECT_SOURCE_DIR}/shell/ShellContext.hpp

        ${PROJECT_SOURCE_DIR}/shell/Alias.cpp
        ${PROJECT_SOURCE_DIR}/shell/Alias.hpp

        ${PROJECT_SOURCE_DIR}/shell/Cmd.hpp
        ${PROJECT_SOURCE_DIR}/shell/Cmd.cpp

        ${PROJECT_SOURCE_DIR}/shell/Executor.hpp
        ${PROJECT_SOURCE_DIR}/shell/Executor.cpp

        ${PROJECT_SOURCE_DIR}/shell/Logger.cpp
        ${PROJECT_SOURCE_DIR}/shell/Logger.hpp

        tokenizer_test.cpp
        cmd_assembler_test.cpp
        )

add_executable(lsh_tests ${SRC})
target_link_libraries(lsh_tests PRIVATE ncurses)
target_include_directories(lsh_tests PRIVATE ${PROJECT_SOURCE_DIR}/shell)