#include "CmdHandler.hpp"
#include "Executor.hpp"
#include "Logger.hpp"
#include "ShellContext.hpp"
#include "Parser.hpp"
#include "User.hpp"
#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>
#include <unistd.h>

using namespace LakeShell;

void LakeShell::Cmd::CommandHandler::handle_commands(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds)
{
    auto copy_if_is_external_cmd = [&](const std::shared_ptr<LakeShell::Cmd::Command>& cmd) {
        return !is_own_cmd(cmd);
    };

    bool was_handled = false;

    for (auto& cmd : cmds) {
        if (is_own_cmd(cmd)) {
            handle_own_cmd(cmd);
            was_handled = true;
        }
    }
    if (was_handled) {
        return;
    }

    std::vector<std::shared_ptr<LakeShell::Cmd::Command>> extern_cmds;
    std::copy_if(cmds.begin(), cmds.end(), std::back_inserter(extern_cmds), copy_if_is_external_cmd);
    validate_external_commands(cmds);
    auto resolved_cmds = resolve_aliased_commands(cmds);
    LakeShell::create_executor(cmds)->execute();
}

LakeShell::Cmd::CommandHandler::CommandHandler(std::shared_ptr<ShellContext>& shell_context)
    : m_shell_context(shell_context)
{
    index_path();
}

bool LakeShell::Cmd::CommandHandler::is_own_cmd(const std::shared_ptr<LakeShell::Cmd::Command>& cmd)
{
    return std::find(m_own_cmds.begin(), m_own_cmds.end(), cmd->get_name()) != m_own_cmds.end();
}

void LakeShell::Cmd::CommandHandler::handle_own_cmd(const std::shared_ptr<LakeShell::Cmd::Command>& cmd)
{
    bool was_executed = false;
    auto cmd_name = cmd->get_name();
    auto args = cmd->get_args();
    if (cmd_name == PWD) {
        cmd->ensure_has_args(0);
        std::string wd = LakeShell::User::current_wd();
        std::cout << wd << '\n'
                  << std::flush;
        was_executed = true;
    } else if (cmd_name == CD) {
        if (args.empty()) {
            std::string usr_home = LakeShell::User::usr_home_dir();
            chdir(usr_home.c_str());
        } else {
            cmd->ensure_has_args(1);
            dbg("got cmd with arg ", cmd->get_args()[0]);
            auto resolve_path = [](std::string& origin_path) {
                std::string wd = LakeShell::User::current_wd();
                if (origin_path == "..") {
                    return wd.substr(0, wd.rfind('/'));
                } else if ((origin_path[0] != '.' && origin_path[0] != '/') || (origin_path[0] == '.' && origin_path[1] == '/')) {
                    return wd + "/" + origin_path;
                }
                assert(origin_path[0] == '/');
                return origin_path;
            };
            std::string path_string = resolve_path(args[0]);
            chdir(path_string.c_str());
            return;
        }
        was_executed = true;
    } else if (cmd_name == ALIAS) {
        cmd->ensure_has_args(2);
        m_shell_context->add_alias(args[0], args[1]);
        was_executed = true;
    } else if (cmd_name == EXPORT) {
        cmd->ensure_has_args(2);
        std::string arg = args[0];
        auto key = args[0].c_str();
        auto value = args[1].c_str();
        setenv(key, value, true);
        was_executed = true;
    } else if (cmd_name == UNEXPORT) {
        cmd->ensure_has_args(1);
        std::string arg = args[0];
        unsetenv(arg.c_str());
        was_executed = true;
    } else if (cmd_name == ECHO) {
        cmd->ensure_has_args(1);
        std::string arg = args[0];
        if (arg[0] == '$') {
            auto key = arg.substr(1, arg.length());
            std::string value(getenv(key.c_str()));
            arg = value;
        }
        std::cout << arg << '\n' << std::flush;
        was_executed = true;
    }
    if (!was_executed)
        throw CommandNotFoundException(cmd_name);
}

bool LakeShell::Cmd::CommandHandler::external_cmd_exists(std::shared_ptr<LakeShell::Cmd::Command> cmd)
{
    auto name = cmd->get_name();
    bool exists_in_path = std::find(m_available_commands.begin(), m_available_commands.end(), name) != m_available_commands.end();
    bool is_alias = m_shell_context->alias_exists(name);
    return exists_in_path || is_alias;
}

void LakeShell::Cmd::CommandHandler::validate_external_commands(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds)
{
    for (auto& cmd : cmds) {
        if (!external_cmd_exists(cmd)) {
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
    // todo - this could maybe be done in parallel
    while ((pos = path.find(delim)) != std::string::npos) {
        token = path.substr(0, pos);
        for (const auto& entry : std::filesystem::directory_iterator(token))
            m_available_commands.push_back(entry.path().filename().string());
        path.erase(0, pos + delim.length());
    }
}

std::vector<std::shared_ptr<LakeShell::Cmd::Command>> Cmd::CommandHandler::resolve_aliased_commands(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds)
{
    std::vector<std::shared_ptr<LakeShell::Cmd::Command>> resolved_cmds;
    for (auto& cmd : cmds) {
        auto cmd_name = cmd->get_name();
        if (m_shell_context->alias_exists(cmd_name)) {
            auto assoc_cmd = m_shell_context->get_origin_of_alias(cmd_name);
            auto result = LakeShell::Parser::parse_input(assoc_cmd);
            for (auto& r : result) {
                resolved_cmds.push_back(r);
            }
        } else {
            resolved_cmds.push_back(cmd);
        }
    }
    return resolved_cmds;
}
