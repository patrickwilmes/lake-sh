#include "CmdHandler.hpp"
#include "Executor.hpp"
#include "Parser.hpp"
#include "ShellContext.hpp"
#include <algorithm>
#include <filesystem>
#include <string>

using namespace LakeShell;

void LakeShell::Cmd::CommandHandler::handle_commands(const std::shared_ptr<LakeShell::Cmd::CommandContainer>& cmd_container)
{
    validate_external_commands(cmd_container->get_commands());
    auto resolved_cmds = resolve_aliased_commands(cmd_container->get_commands());
    if (!resolved_cmds.empty()) {
        if (cmd_container->is_concat()) {
            for (const auto& cmd : resolved_cmds) {
                LakeShell::create_executor(cmd, m_shell_context)->execute();
            }
        } else {
            LakeShell::create_executor(resolved_cmds, m_shell_context)->execute();
        }
    } else {
        if (cmd_container->is_concat()) {
            for (const auto& cmd : cmd_container->get_commands()) {
                LakeShell::create_executor(cmd, m_shell_context)->execute();
            }
        } else {
            LakeShell::create_executor(cmd_container->get_commands(), m_shell_context)->execute();
        }
    }
}

LakeShell::Cmd::CommandHandler::CommandHandler(std::shared_ptr<ShellContext>& shell_context)
    : m_shell_context(shell_context)
{
    index_path();
}

bool LakeShell::Cmd::CommandHandler::external_cmd_exists(const std::shared_ptr<LakeShell::Cmd::Command>& cmd)
{
    auto name = cmd->get_name();
    bool exists_in_path = std::find(m_available_commands.begin(), m_available_commands.end(), name) != m_available_commands.end();
    bool is_alias = m_shell_context->alias_exists(name);
    return exists_in_path || is_alias;
}

void LakeShell::Cmd::CommandHandler::validate_external_commands(const std::vector<std::shared_ptr<LakeShell::Cmd::Command>>& cmds)
{
    for (auto& cmd : cmds) {
        if (!external_cmd_exists(cmd) && !cmd->is_internal_command()) {
            throw CommandNotFoundException(cmd->get_name());
        }
    }
}

void LakeShell::Cmd::CommandHandler::index_path()
{
    std::string path(getenv("PATH"));
    const std::string delim = ":";
    size_t pos;
    std::string token;

    // TODO: this could maybe be done in parallel
    while ((pos = path.find(delim)) != std::string::npos) {
        token = path.substr(0, pos);
        if (token.starts_with("/mnt")) {
            path.erase(0, pos + delim.length());
            continue;
        }
        for (const auto& entry : std::filesystem::directory_iterator(token)) {
            m_available_commands.push_back(entry.path().filename().string());
        }
        path.erase(0, pos + delim.length());
    }
}

std::vector<std::shared_ptr<LakeShell::Cmd::Command>> Cmd::CommandHandler::resolve_aliased_commands(const std::vector<std::shared_ptr<LakeShell::Cmd::Command>>& cmds)
{
    std::vector<std::shared_ptr<LakeShell::Cmd::Command>> resolved_cmds;
    for (auto& cmd : cmds) {
        auto cmd_name = cmd->get_name();
        if (m_shell_context->alias_exists(cmd_name)) {
            auto assoc_cmd = m_shell_context->get_origin_of_alias(cmd_name);
            auto result = LakeShell::Parser::parse_input(assoc_cmd);
            for (auto& r : result->get_commands()) {
                resolved_cmds.push_back(r);
            }
        } else {
            resolved_cmds.push_back(cmd);
        }
    }
    return resolved_cmds;
}
