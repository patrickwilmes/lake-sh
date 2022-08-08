#include "Cmd.hpp"

#include <utility>

LakeShell::Cmd::Command::Command(std::string name)
    : m_name(std::move(name))
{
    // correcting cd.. to cd ..
    if (m_name.starts_with("cd") && m_name.ends_with("..")) {
        m_name = "cd";
        add_arg("..");
    }
}

void LakeShell::Cmd::Command::add_arg(const std::string& arg)
{
    m_args.push_back(arg);
}

void LakeShell::Cmd::Command::ensure_has_args(const uint32_t expected_number)
{
    if (expected_number != m_args.size()) {
        throw InvalidCommandException(m_name.c_str());
    }
}

std::string LakeShell::Cmd::Command::get_name()
{
    return m_name;
}

std::vector<std::string> LakeShell::Cmd::Command::get_args()
{
    return m_args;
}

uint32_t LakeShell::Cmd::Command::arg_count()
{
    return m_args.size();
}

bool LakeShell::Cmd::Command::is_internal_command()
{
    return std::find(m_own_cmds.begin(), m_own_cmds.end(), get_name()) != m_own_cmds.end();
}

std::string LakeShell::Cmd::Command::assemble_alias()
{
    std::string command;
    uint8_t c = 0;
    for (auto& arg: m_args) {
        if (c > 0) { // we dont want to take the first arg as it will be the command name itself
            command += " " + arg;
        }
        c++;
    }
    return command;
}

bool LakeShell::Cmd::Command::is_non_parallel_command()
{
    return get_name() != "ls";
}
