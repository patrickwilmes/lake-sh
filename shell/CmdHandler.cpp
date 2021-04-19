#include "CmdHandler.hpp"
#include "CmdAssembler.hpp"
#include "Executor.hpp"
#include "Logger.hpp"
#include "ShellContext.hpp"
#include "Tokenizer.hpp"
#include "User.hpp"
#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

using namespace LakeShell;

std::string LakeShell::Cmd::CommandHandler::handle_commands(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds)
{
    auto copy_if_is_external_cmd = [&](const std::shared_ptr<LakeShell::Cmd::Command>& cmd) {
        return !is_own_cmd(cmd);
    };

    for (auto& cmd : cmds) {
        if (is_own_cmd(cmd)) {
            return handle_own_cmd(cmd);
        }
    }

    std::vector<std::shared_ptr<LakeShell::Cmd::Command>> extern_cmds;
    std::copy_if(cmds.begin(), cmds.end(), std::back_inserter(extern_cmds), copy_if_is_external_cmd);
    validate_external_commands(cmds);
    auto resolved_cmds = resolve_aliased_commands(cmds);
    return handle_extern_cmds(resolved_cmds);
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

std::string LakeShell::Cmd::CommandHandler::handle_own_cmd(const std::shared_ptr<LakeShell::Cmd::Command>& cmd)
{
    auto cmd_name = cmd->get_name();
    auto args = cmd->get_args();
    if (cmd_name == PWD) {
        std::string wd = LakeShell::User::current_wd();
        return wd;
    } else if (cmd_name == CD) {
        if (args.empty()) {
            std::string usr_home = LakeShell::User::usr_home_dir();
            chdir(usr_home.c_str());
        } else {
            cmd->ensure_has_args(1);
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
            return "";
        }
    } else if (cmd_name == ALIAS) {
        cmd->ensure_has_args(2);
        LakeShell::dbg("printing shit", args[0], args[1]);
        m_shell_context->add_alias(args[0], args[1]);
        return "";
    } else if (cmd_name == EXPORT) {
        cmd->ensure_has_args(2);
        std::string arg = args[0];
        auto key = args[0].c_str();
        auto value = args[1].c_str();
        setenv(key, value, true);
        return "";
    } else if (cmd_name == UNEXPORT) {
        cmd->ensure_has_args(1);
        std::string arg = args[0];
        unsetenv(arg.c_str());
        return "";
    } else if (cmd_name == ECHO) {
        cmd->ensure_has_args(1);
        std::string arg = args[0];
        if (arg[0] == '$') {
            auto key = arg.substr(1, arg.length());
            std::string value(getenv(key.c_str()));
            return value;
        }
        return arg;
    }
    throw CommandNotFoundException(cmd_name.c_str());
}

std::string LakeShell::Cmd::CommandHandler::handle_extern_cmds(const std::vector<std::shared_ptr<LakeShell::Cmd::Command>>& cmds)
{
    if (cmds.size() == 1) {
        // todo - make aliasing happening here
        Executor exec(cmds.front());
        return exec.execute();
    }
    PipedExecutor executor(cmds);
    return executor.execute();
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
            throw CommandNotFoundException(cmd->get_name().c_str());
        }
    }
}

void LakeShell::Cmd::CommandHandler::index_path()
{
    std::string path(getenv("PATH"));
    const std::string delim = ":";

    size_t pos;
    std::string token;
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
            auto tokens = LakeShell::Tokenizer::tokenize(assoc_cmd);
            auto result = LakeShell::Assembler::assemble_commands(tokens);
            for (auto& r : result) {
                resolved_cmds.push_back(r);
            }
        } else {
            resolved_cmds.push_back(cmd);
        }
    }
    return resolved_cmds;
}
