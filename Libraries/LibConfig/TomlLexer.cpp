#include "TomlLexer.h"

#include <IO.h>
#include <iostream>
#include <string>
#include <utility>

LibConfig::TomlLexer::TomlLexer(std::string config_string)
    : m_config_string(std::move(config_string))
{
}

std::vector<LibConfig::Token> LibConfig::TomlLexer::lex()
{
    std::vector<Token> tokens;
    std::string current_token;
    for (size_t i = 0; i < m_config_string.size(); ++i) {
        current_token += m_config_string[i];

        if (current_token == "\n") {
            current_token = "";
            continue;
        }

        if (current_token == "[") {
            tokens.push_back(Token { TokenType::OpenBrace, current_token });
            current_token = "";
            continue;
        }
        if (current_token == "]") {
            tokens.push_back(Token { TokenType::CloseBrace, current_token });
            current_token = "";
            continue;
        }
        if (m_config_string[i + 1] == ']') {
            tokens.push_back(Token { TokenType::Identifier, current_token });
            current_token = "";
        }
    }
    return tokens;
}
