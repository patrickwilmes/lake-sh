#pragma once
#include <string>

namespace LibConfig {
    enum class TokenType {
        Identifier,
        Key,
        String,
        Number,
        EQ,
        OpenBrace,
        CloseBrace,
    };

    struct Token {
        TokenType type;
        std::string value;
    };
}