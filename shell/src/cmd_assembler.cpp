#include "cmd_assembler.hpp"

using namespace lsh::assembler;

void parse_cmd(std::vector<std::string> &tokens, uint32_t parse_pos, std::vector<cmd *> &cmds);
void parse_arg(std::vector<std::string> &tokens, uint32_t parse_pos, cmd *c);

std::vector<lsh::assembler::cmd *> lsh::assembler::assemble_commands(std::vector<std::string> tokens) {
    if (tokens.empty()) {
        return std::vector<cmd *>();
    }
    std::vector<lsh::assembler::cmd *> cmds;
    parse_cmd(tokens, 0, cmds);
    return cmds;
}

void lsh::assembler::free_commands(std::vector<cmd *> cmds) {
    std::for_each(cmds.begin(), cmds.end(), [](cmd *cmd) {
        std::vector<arg *> args = cmd->args;
        std::for_each(args.begin(), args.end(), [](arg *arg) {
            delete arg;
        });
        delete cmd;
    });
}

void parse_cmd(std::vector<std::string> &tokens, uint32_t parse_pos, std::vector<cmd *> &cmds) {
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
    arg *a = new arg;
    a->name = tokens[parse_pos];
    a->value = tokens[parse_pos + 1];
    parse_pos += 2;
    c->args.emplace_back(a);
    parse_arg(tokens, parse_pos, c);
}
