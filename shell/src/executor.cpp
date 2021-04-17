#include "executor.hpp"
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <filesystem>

lsh::piped_executor::piped_executor(std::vector<std::shared_ptr<lsh::cmd::command>> cmds) : m_cmds(std::move(cmds)) {}

std::string lsh::piped_executor::execute() {
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

int lsh::piped_executor::spawn_command(std::shared_ptr<lsh::cmd::command> cmd, int input, bool is_first, bool is_last) {
    int pid;
    int fd[2];
    pipe(fd);

    pid = fork();

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
        args[i] = NULL;
        if (execvp(cmd_name.c_str(), args) == -1) {
            exit(-1);
        }
    }

    if (input != 0) {
        close(input);
    }

    close(fd[1]);

    return fd[0];
}
