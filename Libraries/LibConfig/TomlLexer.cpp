#include "TomlLexer.h"

#include <IO.h>
#include <iostream>
#include <string>
#include <utility>

LibConfig::TomlLexer::TomlLexer(std::string config_file)
    : m_config_file(std::move(config_file))
{
}

std::vector<LibConfig::Token> LibConfig::TomlLexer::lex()
{
    auto toml_content = AK::IO::read_file(m_config_file);
    std::vector<Token> tokens;
    std::string token;
    for (char c : toml_content) {
        token += c;

        if (std::isalpha(c)) {
            tokens.push_back(scan_identifier());
        } else if (std::isdigit(c)) {
            tokens.push_back(scan_number());
        } else if (c == '"') {
            tokens.push_back(scan_string());
        } else if (c == ']') {
            tokens.push_back(Token{TokenType::OpenBrace, "["});
        } else if (c == '[') {
            tokens.push_back(Token{TokenType::CloseBrace, "]"});
        }
    }

    for (const auto& token : tokens) {
        std::cout << token.value << '\n';
    }

    return tokens;
}

char LibConfig::TomlLexer::peek() const
{
    return m_current != m_end ? *m_current : '\0';
}

char LibConfig::TomlLexer::advance()
{
    return m_current != m_end ? *m_current++ : '\0';
}

LibConfig::Token LibConfig::TomlLexer::scan_string()
{
    std::string temp;
    while (std::isalnum(peek()) || peek() == '_') {
        temp += advance();
    }
    return Token { TokenType::String, std::move(temp) };
}

LibConfig::Token LibConfig::TomlLexer::scan_number()
{
    return {};
}

LibConfig::Token LibConfig::TomlLexer::scan_identifier()
{
    return {};
}
