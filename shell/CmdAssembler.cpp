#include "CmdAssembler.hpp"

using namespace LakeShell::Assembler;
using namespace LakeShell::Cmd;

void parse_cmd(std::vector<std::string>& tokens, uint32_t& parse_pos, std::vector<std::shared_ptr<Command>>& cmds);
void parse_arg(std::vector<std::string>& tokens, uint32_t& parse_pos, std::shared_ptr<Command> cmd);

std::vector<std::shared_ptr<Command>> LakeShell::Assembler::assemble_commands(std::vector<std::string> tokens)
{
    if (tokens.empty()) {
        return std::vector<std::shared_ptr<Command>>();
    }
    std::vector<std::shared_ptr<Command>> cmds;
    uint32_t parse_pos = 0;
    parse_cmd(tokens, parse_pos, cmds);
    while (parse_pos < tokens.size()) {
        parse_pos++;
        parse_cmd(tokens, parse_pos, cmds);
    }
    return cmds;
}

void parse_cmd(std::vector<std::string>& tokens, uint32_t& parse_pos, std::vector<std::shared_ptr<Command>>& cmds)
{
    std::string command_name = tokens[parse_pos];
    std::shared_ptr<Command> parsed_command = std::make_shared<Command>(command_name);
    ++parse_pos;
    parse_arg(tokens, parse_pos, parsed_command);
    cmds.emplace_back(parsed_command);
}

void parse_arg(std::vector<std::string>& tokens, uint32_t& parse_pos, std::shared_ptr<Command> cmd)
{
    if (parse_pos > tokens.size() - 1 || tokens[parse_pos] == "|") {
        return;
    }
    cmd->add_arg(tokens[parse_pos]);
    parse_pos++;
    parse_arg(tokens, parse_pos, cmd);
}
