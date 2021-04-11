set(SRC
        src/lsh.cpp
        src/lsh.hpp

        src/tokenizer.cpp
        src/tokenizer.hpp

        src/usr.cpp
        src/usr.hpp

        src/main.cpp
        )

set(TST_SRC
        src/lsh.cpp
        src/lsh.hpp

        src/tokenizer.cpp
        src/tokenizer.hpp

        test/catch2.hpp
        test/test_main.cpp
        test/tokenizer_test.cpp
        )

add_executable(lsh ${SRC})

add_executable(lsh_test ${TST_SRC})
target_include_directories(lsh_test PRIVATE src/)