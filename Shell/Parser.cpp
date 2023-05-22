#include "Parser.hpp"

#include <utility>

using namespace LakeShell::Cmd;

class Tokenizer final {
public:
    explicit Tokenizer(std::string line)
        : m_line(std::move(line))
    {
    }

    std::vector<std::string> tokenize()
    {
        do {
            if (peek() == ' ') {
                consume();
                walk(2);
                continue;
            } else if (peek() == '=') {
                consume();
                walk();
                if (peek() == '"') {
                    walk();
                }
            } else if (peek() == '"') {
                consume();
                walk(2);
            }
            if (m_line[m_pos] != '"')
                m_current_token += m_line[m_pos];
            walk();
        } while (m_pos < m_line.length());
        consume();
        return m_tokens;
    }

private:
    void walk(uint32_t step = 1)
    {
        m_pos += step;
    }

    void consume()
    {
        if (m_pos < m_line.length())
            m_current_token += m_line[m_pos];
        if (m_current_token[0] != '\0')
            m_tokens.emplace_back(m_current_token);
        m_current_token = "";
    }

    char peek()
    {
        return m_line[m_pos + 1];
    }

private:
    uint32_t m_pos = 0;
    std::string m_current_token;
    std::string m_line;
    std::vector<std::string> m_tokens;
};

void parse_cmd(std::vector<std::string>& tokens, uint32_t& parse_pos, std::vector<std::shared_ptr<Command>>& cmds);
void parse_arg(std::vector<std::string>& tokens, uint32_t& parse_pos, const std::shared_ptr<Command>& cmd);

std::shared_ptr<LakeShell::Cmd::CommandContainer> LakeShell::Parser::parse_input(std::string& input)
{
    bool is_concat = false;
    if (input.find("&&") != std::string::npos)
        is_concat = true;
    Tokenizer tokenizer(std::move(input));
    auto tokens = tokenizer.tokenize();
    if (tokens.empty()) {
        return {};
    }
    std::vector<std::shared_ptr<Command>> cmds;
    uint32_t parse_pos = 0;
    parse_cmd(tokens, parse_pos, cmds);
    while (parse_pos < tokens.size()) {
        parse_pos++;
        parse_cmd(tokens, parse_pos, cmds);
    }
    return std::make_shared<CommandContainer>(cmds, is_concat);
}

void parse_cmd(std::vector<std::string>& tokens, uint32_t& parse_pos, std::vector<std::shared_ptr<Command>>& cmds)
{
    std::string command_name = tokens[parse_pos];
    std::shared_ptr<Command> parsed_command = std::make_shared<Command>(command_name);
    ++parse_pos;
    parse_arg(tokens, parse_pos, parsed_command);
    cmds.emplace_back(parsed_command);
}

void parse_arg(std::vector<std::string>& tokens, uint32_t& parse_pos, const std::shared_ptr<Command>& cmd)
{
    if (parse_pos > tokens.size() - 1 || tokens[parse_pos] == "|" || tokens[parse_pos] == "&&") {
        return;
    }
    cmd->add_arg(tokens[parse_pos]);
    parse_pos++;
    parse_arg(tokens, parse_pos, cmd);
}
