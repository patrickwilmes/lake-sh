#include "TomlLexer.h"

#include "Utils.hpp"
#include <iostream>
#include <string>
#include <utility>

LibConfig::TomlLexer::TomlLexer(std::string config_string)
    : m_config_string(std::move(config_string))
{
}

std::vector<LibConfig::Token> LibConfig::TomlLexer::lex()
{
    while (m_pos < m_config_string.size()) {
        switch (m_state) {
        case State::Default:
            if (peek() == '[') {
                m_state = State::Section;
            } else {
                m_pos++;
            }
            break;
        case State::Section:
            consume_section();
            m_state = State::KeyValue;
            break;
        case State::KeyValue:
            consume_key_value();
            m_state = State::Default;
            break;
        }
    }
    return m_tokens;
}

void LibConfig::TomlLexer::consume_section()
{
    m_tokens.push_back(Token{ TokenType::OpenBrace, "[" });
    m_pos++;
    std::string section_name;
    while (peek() != ']') {
        section_name += m_config_string[m_pos];
        m_pos++;
    }
    m_tokens.push_back(Token{ TokenType::Identifier, section_name });
    m_tokens.push_back(Token{ TokenType::CloseBrace, "]" });
    m_pos = m_pos+2;
}

void LibConfig::TomlLexer::consume_key_value()
{
    std::string key;
    while (peek() != '=') {
        key += m_config_string[m_pos];
        m_pos++;
    }
    m_tokens.push_back(Token{ TokenType::Key, trim_copy(key)});
    m_tokens.push_back(Token{ TokenType::EQ, "=" });
    m_pos++;
    std::string value;
    while (peek() != '\n') {
        if (m_config_string[m_pos] == '"') {
            m_pos++;
            continue;
        }
        value += m_config_string[m_pos];
        m_pos++;
    }
    m_tokens.push_back(Token{ TokenType::String, trim_copy(value) });
    m_pos++;
}

char LibConfig::TomlLexer::peek() const
{
    return m_config_string[m_pos];
}
