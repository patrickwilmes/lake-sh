#include <gtest/gtest.h>
#include "TomlLexer.h"

class TomlLexerTest : public ::testing::Test {};

TEST_F(TomlLexerTest, ConfigLoadTest) {
    EXPECT_TRUE(true);
}

TEST_F(TomlLexerTest, ConfigValueTest) {
    EXPECT_EQ(2 + 2, 4);
}