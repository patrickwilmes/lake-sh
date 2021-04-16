#ifndef LAKE_SH_TOKENIZER_HPP
#define LAKE_SH_TOKENIZER_HPP

#include <string>
#include <vector>

namespace lsh {
    class lsh_tokenizer final {
    public:
    private:
    };
    namespace tokenizer {
        [[deprecated]]
        std::vector<std::string> tokenize(std::string &line);
    }
}

#endif//LAKE_SH_TOKENIZER_HPP
