#include "Executor.hpp"
#include <Terminal.hpp>
#include <filesystem>
#include <unistd.h>
#include <sys/wait.h>
#include <utility>

LakeShell::PipedExecutor::PipedExecutor(std::vector<std::shared_ptr<LakeShell::Cmd::Command>> cmds) : m_cmds(std::move(cmds)) {}

std::string LakeShell::PipedExecutor::execute() {
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

int LakeShell::PipedExecutor::spawn_command(std::shared_ptr<LakeShell::Cmd::Command> cmd, int input, bool is_first, bool is_last) {
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
            dup2(fd[1], STDOUT_FILENO);// exp
        } else {
            dup2(input, STDIN_FILENO);
            dup2(fd[1], STDOUT_FILENO);
        }
        auto cmd_name = cmd->get_name();
        const char *c = cmd_name.c_str();
        char *args[cmd->arg_count() + 2];
        args[0] = const_cast<char *>(c);
        int i = 1;
        for (auto &arg : cmd->get_args()) {
            args[i] = const_cast<char *>(arg.c_str());
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
    const char *c = cmd_name.c_str();
    char *args[m_cmd->arg_count() + 2];
    args[0] = const_cast<char *>(c);
    int i = 1;
    for (auto &arg : m_cmd->get_args()) {
        args[i] = const_cast<char *>(arg.c_str());
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
        if (m_cmd->get_name() != "clear") {
            close(fd[0]);
            dup2(fd[1], 1);
            close(fd[1]);
        }
        execvp(c, args);
        throw std::runtime_error("failed to execute Command!");
    } else {
        do {
            p = wait(&child_status);
        } while (p != pid);
        if (m_cmd->get_name() != "clear") {
            char buffer[2048];
            for (char & b : buffer) {
                b = 0;
            }
            close(fd[1]);
            while (read(fd[0], buffer, sizeof(buffer)) != 0)
                ;
            std::string data(buffer);
            return data;
        }
    }
    return std::string();
}

LakeShell::Executor::Executor(std::shared_ptr<LakeShell::Cmd::Command> cmd): m_cmd(std::move(cmd))
{
}
