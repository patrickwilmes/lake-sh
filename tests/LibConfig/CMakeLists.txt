enable_testing()

add_executable(
        libconfig_test
        TomlLexerTest.cpp
        TomlParserTest.cpp
)
target_link_libraries(
        libconfig_test
        GTest::gtest_main
        LibConfig
)

include(GoogleTest)
gtest_discover_tests(libconfig_test)