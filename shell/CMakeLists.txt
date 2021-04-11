set(SRC
        src/lsh.cpp
        src/lsh.hpp

        src/tokenizer.cpp
        src/tokenizer.hpp

        src/usr.cpp
        src/usr.hpp

        src/cmd_assembler.cpp
        src/cmd_assembler.hpp

        src/cmd_handler.cpp
        src/cmd_handler.hpp

        src/cmd/cmd.hpp
        src/cmd/cmd_pwd.cpp
        src/cmd/cmd_cd.cpp
        )

set(TST_SRC
        test/catch2.hpp
        test/test_main.cpp
        test/tokenizer_test.cpp
        test/cmd_assembler_test.cpp
        )

add_executable(lsh src/main.cpp ${SRC} )
target_include_directories(lsh PRIVATE src)

add_executable(lsh_test ${SRC} ${TST_SRC})
target_include_directories(lsh_test PRIVATE src/)