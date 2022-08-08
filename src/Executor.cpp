#include "Executor.hpp"
#include "User.hpp"
#include <cassert>
#include <filesystem>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>

LakeShell::PipedExecutor::PipedExecutor(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds)
    : m_cmds(std::move(cmds))
{
}

std::string LakeShell::PipedExecutor::execute()
{
    int input = 0;
    int num_cmds = m_cmds.size();
    for (int i = 0; i < num_cmds; i++) {
        input = spawn_command(m_cmds[i], input, i == 0, i == num_cmds - 1);
    }
    char buffer[2048];
    while (read(input, buffer, sizeof(buffer)) != 0)
        ;
    std::string data(buffer);
    close(input);
    return data;
}

//TODO: pass cmd as const ref
int LakeShell::PipedExecutor::spawn_command(std::shared_ptr<LakeShell::Cmd::Command> cmd, int input, bool is_first, bool is_last)
{
    int pid;
    int fd[2];
    pipe(fd);

    if ((pid = fork()) < 0) {
        throw std::runtime_error("failed to fork process!");
    }

    if (pid == 0) {
        if (is_first) {
            dup2(fd[1], STDOUT_FILENO);
        } else if (is_last) {
            dup2(input, STDIN_FILENO);
        } else {
            dup2(input, STDIN_FILENO);
            dup2(fd[1], STDOUT_FILENO);
        }
        auto cmd_name = cmd->get_name();
        const char* c = cmd_name.c_str();
        char* args[cmd->arg_count() + 2];
        args[0] = const_cast<char*>(c);
        int i = 1;
        for (auto& arg : cmd->get_args()) {
            args[i] = const_cast<char*>(arg.c_str());
            i++;
        }
        args[i] = nullptr;
        if (execvp(cmd_name.c_str(), args) == -1) {
            throw std::runtime_error("failed to execute Command!");
        }
    }

    if (input != 0) {
        close(input);
    }

    close(fd[1]);

    return fd[0];
}

std::string LakeShell::Executor::execute()
{
    int fd[2];
    auto cmd_name = m_cmd->get_name();
    int child_status;
    const char* c = cmd_name.c_str();
    char* args[m_cmd->arg_count() + 2];
    args[0] = const_cast<char*>(c);
    int i = 1;
    for (auto& arg : m_cmd->get_args()) {
        args[i] = const_cast<char*>(arg.c_str());
        i++;
    }
    args[i] = nullptr;
    if (pipe(fd) == -1) {
        throw std::runtime_error("failed to initialize pipe!");
    }
    pid_t p;
    pid_t pid = fork();
    if (pid < 0) {
        throw std::runtime_error("failed to fork process");
    } else if (pid == 0) {
        execvp(c, args);
        throw std::runtime_error("failed to execute Command!");
    } else {
        do {
            p = wait(&child_status);
        } while (p != pid);
    }
    return std::string();
}

LakeShell::Executor::Executor(std::shared_ptr<LakeShell::Cmd::Command> cmd)
    : m_cmd(std::move(cmd))
{
}

std::shared_ptr<LakeShell::Executor> LakeShell::create_executor(const std::vector<std::shared_ptr<LakeShell::Cmd::Command>>& commands, const std::shared_ptr<LakeShell::ShellContext>& ctx)
{
    if (commands.size() == 1) {
        auto cmd = commands.front();
        if (cmd->is_internal_command())
            return std::make_shared<LakeShell::OwnCommandExecutor>(cmd, ctx);
        return std::make_shared<LakeShell::Executor>(cmd);
    }
    return std::make_shared<LakeShell::PipedExecutor>(commands);
}

std::string LakeShell::OwnCommandExecutor::execute()
{
    bool was_executed = false;
    auto cmd_name = m_cmd->get_name();
    auto args = m_cmd->get_args();
    if (cmd_name == PWD) {
        m_cmd->ensure_has_args(0);
        std::string wd = LakeShell::User::current_wd();
        std::cout << wd << '\n'
                  << std::flush;
        was_executed = true;
    } else if (cmd_name == CD) {
        if (args.empty()) {
            std::string usr_home = LakeShell::User::usr_home_dir();
            chdir(usr_home.c_str());
        } else {
            m_cmd->ensure_has_args(1);
            auto resolve_path = [](std::string& origin_path) {
                std::string wd = LakeShell::User::current_wd();
                if (origin_path == "..") {
                    std::filesystem::path opath = origin_path;
                    return opath.string();
                } else if ((origin_path[0] != '.' && origin_path[0] != '/') || (origin_path[0] == '.' && origin_path[1] == '/')) {
                    return wd + "/" + origin_path;
                }
                assert(origin_path[0] == '/');
                return origin_path;
            };
            std::string path_string = resolve_path(args[0]);
            chdir(path_string.c_str());
        }
        was_executed = true;
    } else if (cmd_name == ALIAS) {
        printf("Assembled arg: %s\n", m_cmd->assemble_alias().c_str());
        m_cmd->ensure_has_args(3);
        m_shell_context->add_alias(args[0], m_cmd->assemble_alias());
        was_executed = true;
    } else if (cmd_name == EXPORT) {
        m_cmd->ensure_has_args(2);
        std::string arg = args[0];
        auto key = args[0].c_str();
        auto value = args[1].c_str();
        setenv(key, value, true);
        was_executed = true;
    } else if (cmd_name == UNEXPORT) {
        m_cmd->ensure_has_args(1);
        std::string arg = args[0];
        unsetenv(arg.c_str());
        was_executed = true;
    } else if (cmd_name == ECHO) {
        m_cmd->ensure_has_args(1);
        std::string arg = args[0];
        if (arg[0] == '$') {
            auto key = arg.substr(1, arg.length());
            std::string value(getenv(key.c_str()));
            arg = value;
        }
        std::cout << arg << '\n'
                  << std::flush;
        was_executed = true;
    }
    if (!was_executed)
        throw LakeShell::Cmd::CommandNotFoundException(cmd_name);
    return "";
}

LakeShell::OwnCommandExecutor::OwnCommandExecutor(std::shared_ptr<LakeShell::Cmd::Command> cmd, std::shared_ptr<LakeShell::ShellContext> shell_context)
    : m_cmd(std::move(cmd))
    , m_shell_context(std::move(shell_context))
{
}
