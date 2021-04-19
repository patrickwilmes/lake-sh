#ifndef LAKE_SH_TOKENIZER_HPP
#define LAKE_SH_TOKENIZER_HPP

#include <string>
#include <vector>

namespace LakeShell::Tokenizer {
    std::vector<std::string> tokenize(std::string& line);
}

#endif //LAKE_SH_TOKENIZER_HPP
