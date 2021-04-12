#include "cmd_assembler.hpp"

using namespace lsh::assembler;

void parse_cmd(std::vector<std::string> &tokens, uint32_t parse_pos, std::vector<std::shared_ptr<cmd>> &cmds);
void parse_arg(std::vector<std::string> &tokens, uint32_t parse_pos, cmd *c);

std::vector<std::shared_ptr<lsh::assembler::cmd>> lsh::assembler::assemble_commands(std::vector<std::string> tokens) {
    if (tokens.empty()) {
        return std::vector<std::shared_ptr<cmd>>();
    }
    std::vector<std::shared_ptr<lsh::assembler::cmd>> cmds;
    parse_cmd(tokens, 0, cmds);
    return cmds;
}

void parse_cmd(std::vector<std::string> &tokens, uint32_t parse_pos, std::vector<std::shared_ptr<cmd>> &cmds) {
    cmd *c = new cmd;
    c->name = tokens[parse_pos];
    ++parse_pos;
    parse_arg(tokens, parse_pos, c);
    cmds.emplace_back(c);
}

void parse_arg(std::vector<std::string> &tokens, uint32_t parse_pos, cmd *c) {
    if (parse_pos > tokens.size() - 1 || tokens[parse_pos] == "|") {
        return;
    }
    parse_pos++;
    c->args.emplace_back(tokens[parse_pos]);
    parse_arg(tokens, parse_pos, c);
}
