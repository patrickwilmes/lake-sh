#include "Tokenizer.h"

#include <utility>
#include <Utils.hpp>

LibRepl::Tokenizer::Tokenizer(std::string input)
    : m_input(std::move(input))
{
}

std::vector<LibRepl::Token> LibRepl::Tokenizer::tokenize()
{
    std::string token;
    for (size_t i = 0; i < m_input.size(); ++i) {
        token += m_input[i];
        if (token == " ") {
            token = "";
            continue;
        }
        if (m_input[i] == ' ' || i == m_input.size() - 1) {
            m_tokens.push_back(Token{ TokenType::Identifier, trim_copy(token) });
            token = "";
            continue;
        }
        if (m_input[i] == '|') {
            m_tokens.push_back(Token{ TokenType::Pipe, "|" });
            token = "";
        }
    }
    return m_tokens;
}
