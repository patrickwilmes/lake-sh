set(SRC
        catch.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/tokenizer.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/tokenizer.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/cmd_assembler.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/cmd_assembler.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/lsh.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/lsh.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/usr.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/usr.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/cmd_handler.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/cmd_handler.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/utils/utils.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/utils/utils.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/history.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/history.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/shell_context.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/shell_context.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/alias.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/alias.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/cmd.hpp
        ${PROJECT_SOURCE_DIR}/shell/src/cmd.cpp

        ${PROJECT_SOURCE_DIR}/shell/src/logger.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/logger.hpp

        tokenizer_test.cpp
        cmd_assembler_test.cpp
        )

add_executable(lsh_tests ${SRC})
target_link_libraries(lsh_tests PRIVATE ncurses)
target_include_directories(lsh_tests PRIVATE ${PROJECT_SOURCE_DIR}/shell/src)