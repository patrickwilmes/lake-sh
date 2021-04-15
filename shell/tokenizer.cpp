#include "tokenizer.hpp"
#include <cassert>

std::vector<std::string> lsh::tokenizer::tokenize(std::string &line) {
    std::vector<std::string> tokens;

    const std::string std_delim = " ";

    size_t pos = 0;
    std::string token;

    while ((pos = line.find(std_delim)) != std::string::npos) {
        if (line[0] != '"') {
            token = line.substr(0, pos);
            line.erase(0, pos + 1);
        } else {
            size_t sdelim_pos = line.find('=');
            if (sdelim_pos < pos) {
                token = line.substr(0, sdelim_pos);
                line.erase(0, sdelim_pos + 1);
            } else {
                size_t end_pos = line.find('"', 1);
                token = line.substr(1, end_pos);
                line.erase(0, end_pos + 1);
            }
        }
        tokens.push_back(token);
    }

    tokens.push_back(line);
    return tokens;
}
