#include "TomlLexer.h"
#include <gtest/gtest.h>

class TomlLexerTest : public ::testing::Test { };

TEST_F(TomlLexerTest, ConfigWithSectionOnly)
{
    LibConfig::TomlLexer lexer(R"(
[Section]
)");
    const auto tokens = lexer.lex();
    EXPECT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].value, "[");
    EXPECT_EQ(tokens[1].value, "Section");
    EXPECT_EQ(tokens[2].value, "]");
}

TEST_F(TomlLexerTest, ConfigWithKeyValueOnly)
{
    LibConfig::TomlLexer lexer(R"(
[Section]
key = "value"
)");

    const auto tokens = lexer.lex();

    EXPECT_EQ(tokens[3].value, "key");
    EXPECT_EQ(tokens[4].value, "=");
    EXPECT_EQ(tokens[5].value, "value");
}
