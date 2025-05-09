#pragma once
#include <string>

namespace LibRepl {
    enum class TokenType {
        Identifier,
        Dollar,
        EQ,
        OpenCurlyBrace,
        CloseCurlyBrace,
        OpenBrace,
        CloseBrace,
        Pipe,
        DoubleQuote,
        SingleQuote
    };

    struct Token {
        TokenType type;
        std::string value;
    };
}