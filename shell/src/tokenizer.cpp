#include "tokenizer.hpp"
#include "utils/utils.hpp"
#include <assert.h>

std::vector<std::string> lsh::tokenizer::tokenize(std::string &line) {
    std::vector<std::string> tokens;
    const uint32_t origin_len = line.length();
    const std::string std_delim = " ";

    size_t pos;
    std::string token;

    while ((pos = line.find(std_delim)) != std::string::npos) {
        trim(line);
        if (line[0] != '"') {
            size_t sdelim_pos = line.find('=');
            if (sdelim_pos < pos) {
                token = line.substr(0, sdelim_pos);
                line.erase(0, sdelim_pos + 1);
            } else {
                token = line.substr(0, pos);
                line.erase(0, pos + 1);
            }
        } else {
            size_t end_pos = line.find('"', 1);
            token = line.substr(1, end_pos - 1);
            line.erase(0, end_pos);
        }
        tokens.push_back(token);
    }
    if (line.length() > 2 || line.length() == origin_len)
        tokens.push_back(line);
    return tokens;
}

lsh::lsh_tokenizer::lsh_tokenizer(const std::string input): m_input(input) { }

void lsh::lsh_tokenizer::lex() {
    auto emit_token = [&](const char single_char_token) {
        token_t token = {
            .token = IDENTIFIER,
        };
        m_tokens.push_back(token);
    };
}

char lsh::lsh_tokenizer::consume() {
    return 0;
}

char lsh::lsh_tokenizer::peek() {
    assert(input_idx < m_input.length());
    return m_input[input_idx + 1];
}
