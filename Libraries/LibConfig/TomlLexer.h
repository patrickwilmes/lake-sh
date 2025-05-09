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
        std::vector<Token> m_tokens;
        size_t m_pos = 0;

        enum class State {
            Default,
            Section,
            KeyValue
        };

        State m_state = State::Default;

        void consume_section();
        void consume_key_value();

        [[nodiscard]] char peek() const;
    };
}