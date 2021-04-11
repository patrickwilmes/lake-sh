#include "tokenizer.hpp"
#include <iterator>
#include <sstream>

std::vector<std::string> lsh::tokenizer::tokenize(std::string &line) {
    std::istringstream iss(line);
    std::vector<std::string> results((std::istream_iterator<std::string>(iss)), std::istream_iterator<std::string>());
    return results;
}
