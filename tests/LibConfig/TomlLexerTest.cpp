#include <gtest/gtest.h>
#include "TomlLexer.h"

class TomlLexerTest : public ::testing::Test {};

TEST_F(TomlLexerTest, ConfigWithSectionOnly) {
    LibConfig::TomlLexer lexer(R"(
[Section]
)");
    const auto tokens = lexer.lex();
    EXPECT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].value, "[");
    EXPECT_EQ(tokens[1].value, "Section");
    EXPECT_EQ(tokens[2].value, "]");
}
