#include "Parser.hpp"
#include "utils/Utils.hpp"

using namespace LakeShell::Cmd;

std::vector<std::string> tokenize(std::string& line);

void parse_cmd(std::vector<std::string>& tokens, uint32_t& parse_pos, std::vector<std::shared_ptr<Command>>& cmds);
void parse_arg(std::vector<std::string>& tokens, uint32_t& parse_pos, std::shared_ptr<Command> cmd);

std::vector<std::shared_ptr<LakeShell::Cmd::Command>> LakeShell::Parser::parse_input(std::string &input)
{
    auto tokens = tokenize(input);
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
std::vector<std::string> tokenize(std::string& line)
{
    std::vector<std::string> tokens;
    const uint32_t origin_len = line.length();
    const std::string std_delim = " ";

    size_t pos;
    std::string token;

    while ((pos = line.find(std_delim)) != std::string::npos) {
        trim(line);
        if (line[0] != '"') {
            size_t sdelim_pos = line.find('=');
            if (sdelim_pos < pos) {
                token = line.substr(0, sdelim_pos);
                line.erase(0, sdelim_pos + 1);
            } else {
                token = line.substr(0, pos);
                line.erase(0, pos + 1);
            }
        } else {
            size_t end_pos = line.find('"', 1);
            token = line.substr(1, end_pos - 1);
            line.erase(0, end_pos);
        }
        tokens.push_back(token);
    }
    if (line.length() > 2 || line.length() == origin_len || line == "..")
        tokens.push_back(line);
    return tokens;
}
