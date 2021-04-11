#include "catch2.hpp"
#include <string>
#include "tokenizer.hpp"

using namespace lsh::tokenizer;

TEST_CASE("tokenizer gets empty line returns empty vector") {
    std::string line;
    auto tokens = tokenize(line);

    REQUIRE(tokens.empty());
}

TEST_CASE("tokenizer gets line containing command with no args returns vector with one entry") {
    std::string line = "curl";

    auto tokens = tokenize(line);

    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens[0] == "curl");
}

TEST_CASE("tokenizer gets line containing command with args returns vector with command and args") {
    std::string line = "curl -X post";

    auto tokens = tokenize(line);

    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == "curl");
    REQUIRE(tokens[1] == "-X");
    REQUIRE(tokens[2] == "post");
}
