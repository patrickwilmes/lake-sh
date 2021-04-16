#ifndef LAKE_SH_TOKENIZER_HPP
#define LAKE_SH_TOKENIZER_HPP

#include <string>
#include <vector>

namespace lsh {

    enum lsh_token {
        IDENTIFIER,
        EQUALS,
        DOUBLE_QUOTE
    };

    typedef struct token {
        lsh_token token;
        std::string value;
    }token_t;

    class lsh_tokenizer final {
    public:
        explicit lsh_tokenizer(std::string input);
        void lex();
    private:
        char consume();
        char peek();
    private:
        const uint32_t LOOKAHEAD = 1;
        uint32_t input_idx = 0;
        const std::string m_input;
        std::vector<token_t> m_tokens;
    };

    namespace tokenizer {
        [[deprecated]]
        std::vector<std::string> tokenize(std::string &line);
    }
}

#endif//LAKE_SH_TOKENIZER_HPP
