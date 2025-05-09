#pragma once

#include "Token.h"
#include <string>
#include <vector>

namespace LibConfig {
    class TomlLexer {
    public:
        explicit TomlLexer(std::string  config_string);
        [[nodiscard]] std::vector<Token> lex();
    private:
        std::string m_config_string;
    };
}