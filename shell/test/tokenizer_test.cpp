#ifndef CATCH_CONFIG_MAIN
#define CATCH_CONFIG_MAIN
#endif
#include "catch.hpp"
#include "tokenizer.hpp"
#include <string>

/*
 * begin legacy tests
 */

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

TEST_CASE("lexing an echo command having a space separated text") {
    std::string cmd = "echo \"hello world\"";
    auto tokens = lsh::tokenizer::tokenize(cmd);

    REQUIRE(tokens.size() == 2);
    REQUIRE(tokens[0] == "echo");
    REQUIRE(tokens[1] == "hello world");
}

TEST_CASE("lexing piped commands") {
    std::string cmd = "ls -latr | sort";
    auto tokens = lsh::tokenizer::tokenize(cmd);

    REQUIRE(tokens.size() == 4);
    REQUIRE(tokens[0] == "ls");
    REQUIRE(tokens[1] == "-latr");
    REQUIRE(tokens[2] == "|");
    REQUIRE(tokens[3] == "sort");
}
