#pragma once

#include <string>
#include <vector>
#include "Token.h"

namespace LibRepl {
    class Tokenizer {
    public:
        explicit Tokenizer(std::string input);
        std::vector<Token> tokenize();
    private:
        std::string m_input{};
        std::vector<Token> m_tokens{};
    };
}