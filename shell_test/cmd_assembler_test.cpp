#include "../shell/cmd_assembler.hpp"
#include "catch.hpp"
#include <string>
#include <vector>

using namespace lsh::assembler;

TEST_CASE("given a single token try assemble cmd") {
    std::vector<std::string> tokens = {"ls"};
    auto cmds = assemble_commands(tokens);

    REQUIRE(cmds.size() == 1);
    REQUIRE(cmds[0]->arg_count() == 0);
    REQUIRE(cmds[0]->get_args().empty());
    REQUIRE(cmds[0]->get_name() == "ls");
}

TEST_CASE("given two tokens, a command with a parameter and try to assemble a cmd") {
    std::vector<std::string> tokens = {"ls", "-latr"};
    auto cmds = assemble_commands(tokens);

    REQUIRE(cmds.size() == 1);
    REQUIRE(cmds[0]->get_name() == "ls");
    REQUIRE(cmds[0]->arg_count() == 1);
    REQUIRE(cmds[0]->get_args()[0] == "-latr");
}
