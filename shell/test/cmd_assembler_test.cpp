#include "catch2.hpp"
#include "cmd_assembler.hpp"
#include <vector>
#include <string>

using namespace lsh::assembler;

TEST_CASE("assemble commands with an empty token list") {
    std::vector<std::string> tokens;

    auto result = assemble_commands(tokens);

    REQUIRE(result.empty());
}

TEST_CASE("assemble commands with a single command with no args given") {
    std::vector<std::string> tokens = {"curl"};

    std::vector<cmd *> result = assemble_commands(tokens);

    REQUIRE(result.size() == 1);
    REQUIRE(result[0]->name == "curl");
    REQUIRE(result[0]->args.empty());
    free_commands(result);
}

TEST_CASE("assemble commands with a single command having multiple flag arguments") {
    std::vector<std::string> tokens = {"curl", "-X", "POST", "-H", "test"};

    auto result = assemble_commands(tokens);

    REQUIRE(result.size() == 1);
    REQUIRE(result[0]->name == "curl");
    REQUIRE(result[0]->args[0]->name == "-X");
    REQUIRE(result[0]->args[0]->value == "POST");
    REQUIRE(result[0]->args[1]->name == "-H");
    REQUIRE(result[0]->args[1]->value == "test");
    free_commands(result);
}