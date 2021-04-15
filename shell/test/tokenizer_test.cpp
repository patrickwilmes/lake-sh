#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "catch.hpp"
#include "tokenizer.hpp"
#include <string>

TEST_CASE("lexing a command having an equal sign") {
    std::string cmd = "alias ll=\"ls -latr\"";
    auto tokens = lsh::tokenizer::tokenize(cmd);

    REQUIRE(tokens.size() == 3);
    REQUIRE(tokens[0] == "alias");
    REQUIRE(tokens[1] == "ll");
    REQUIRE(tokens[2] == "ls -latr");
}

TEST_CASE("lexing a simple single word command") {
    std::string cmd = "ls";
    auto tokens = lsh::tokenizer::tokenize(cmd);

    REQUIRE(tokens.size() == 1);
    REQUIRE(tokens[0] == "ls");
}
