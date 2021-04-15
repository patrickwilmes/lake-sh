set(SRC
        catch.hpp

        ${PROJECT_SOURCE_DIR}/shell/src/tokenizer.cpp
        ${PROJECT_SOURCE_DIR}/shell/src/tokenizer.hpp
        tokenizer_test.cpp

        )

add_executable(lsh_tests ${SRC})
target_include_directories(lsh_tests PRIVATE ${PROJECT_SOURCE_DIR}/shell/src)