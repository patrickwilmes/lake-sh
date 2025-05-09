add_executable(
        librepl_test
        TokenizerTest.cpp
)
target_link_libraries(
        librepl_test
        GTest::gtest_main
        LibRepl
)

include(GoogleTest)
gtest_discover_tests(librepl_test)
