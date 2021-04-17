set(SRC
        catch.hpp

        ${PROJECT_SOURCE_DIR}/shell/tokenizer.cpp
        ${PROJECT_SOURCE_DIR}/shell/tokenizer.hpp

        ${PROJECT_SOURCE_DIR}/shell/cmd_assembler.cpp
        ${PROJECT_SOURCE_DIR}/shell/cmd_assembler.hpp

        ${PROJECT_SOURCE_DIR}/shell/lsh.cpp
        ${PROJECT_SOURCE_DIR}/shell/lsh.hpp

        ${PROJECT_SOURCE_DIR}/shell/usr.cpp
        ${PROJECT_SOURCE_DIR}/shell/usr.hpp

        ${PROJECT_SOURCE_DIR}/shell/cmd_handler.cpp
        ${PROJECT_SOURCE_DIR}/shell/cmd_handler.hpp

        ${PROJECT_SOURCE_DIR}/shell/utils/utils.cpp
        ${PROJECT_SOURCE_DIR}/shell/utils/utils.hpp

        ${PROJECT_SOURCE_DIR}/shell/history.cpp
        ${PROJECT_SOURCE_DIR}/shell/history.hpp

        ${PROJECT_SOURCE_DIR}/shell/shell_context.cpp
        ${PROJECT_SOURCE_DIR}/shell/shell_context.hpp

        ${PROJECT_SOURCE_DIR}/shell/alias.cpp
        ${PROJECT_SOURCE_DIR}/shell/alias.hpp

        ${PROJECT_SOURCE_DIR}/shell/cmd.hpp
        ${PROJECT_SOURCE_DIR}/shell/cmd.cpp

        ${PROJECT_SOURCE_DIR}/shell/executor.hpp
        ${PROJECT_SOURCE_DIR}/shell/executor.cpp

        ${PROJECT_SOURCE_DIR}/shell/logger.cpp
        ${PROJECT_SOURCE_DIR}/shell/logger.hpp

        tokenizer_test.cpp
        cmd_assembler_test.cpp
        )

add_executable(lsh_tests ${SRC})
target_link_libraries(lsh_tests PRIVATE ncurses)
target_include_directories(lsh_tests PRIVATE ${PROJECT_SOURCE_DIR}/shell)