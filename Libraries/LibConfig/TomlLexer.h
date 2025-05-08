#pragma once

#include "Token.h"
#include <string>
#include <vector>

namespace LibConfig {
    class TomlLexer {
    public:
        explicit TomlLexer(std::string  config_file);
        [[nodiscard]] std::vector<Token> lex();
    private:
        std::string m_config_file;
        std::string::iterator m_current;
        std::string::iterator m_end;

        [[nodiscard]] char peek() const;

        char advance();

        Token scan_string();
        Token scan_number();
        Token scan_identifier();
    };
}