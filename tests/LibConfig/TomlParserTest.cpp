#include "TomlParser.h"
#include <gtest/gtest.h>

class TomlParserTest : public ::testing::Test { };

TEST_F(TomlParserTest, ParseKeyValuesAndIgnoreSections)
{
    std::vector<LibConfig::Token> tokens;
    tokens.push_back(LibConfig::Token{LibConfig::TokenType::OpenBrace, "["});
    tokens.push_back(LibConfig::Token{LibConfig::TokenType::Identifier, "Section"});
    tokens.push_back(LibConfig::Token{LibConfig::TokenType::CloseBrace, "]"});
    tokens.push_back(LibConfig::Token{LibConfig::TokenType::Key, "key"});
    tokens.push_back(LibConfig::Token{LibConfig::TokenType::EQ, "="});
    tokens.push_back(LibConfig::Token{LibConfig::TokenType::String, "value"});
    LibConfig::TomlParser parser(tokens);
    auto config_store = parser.parse();
    ASSERT_EQ(config_store->get("key"), "value");
}
