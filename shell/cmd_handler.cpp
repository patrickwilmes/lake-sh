#include "cmd_handler.hpp"
#include "cmd/cmd.hpp"
#include <iostream>
#include <ncurses.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>

bool is_own_cmd(const std::shared_ptr<lsh::assembler::cmd> &cmd);
void handle_own_cmd(const std::shared_ptr<lsh::assembler::cmd> &cmd);
std::string handle_extern_cmds(const std::vector<std::shared_ptr<lsh::assembler::cmd>> &cmds);

class piped_executor final {
public:
    explicit piped_executor(std::vector<std::shared_ptr<lsh::assembler::cmd>> cmds) : m_cmds(std::move(cmds)) {}

    std::string execute() {
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

private:
    int spawn_command(std::shared_ptr<lsh::assembler::cmd> cmd, int input, bool is_first, bool is_last) {
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
            const char *c = cmd->name.c_str();
            char *args[cmd->args.size() + 2];// = {const_cast<char *>(c), const_cast<char *>(latr), NULL};
            args[0] = const_cast<char *>(c);
            int i = 1;
            for (auto &arg : cmd->args) {
                args[i] = const_cast<char *>(arg.c_str());
                i++;
            }
            args[i] = NULL;
            if (execvp(cmd->name.c_str(), args) == -1) {
                exit(-1);
            }
        }

        if (input != 0) {
            close(input);
        }

        close(fd[1]);

        //        if (is_last)
        //            close(fd[0]);

        return fd[0];
    }

private:
    const std::vector<std::shared_ptr<lsh::assembler::cmd>> m_cmds;
};


std::string lsh::cmd::handle_commands(std::vector<std::shared_ptr<lsh::assembler::cmd>> cmds) {
    auto copy_if_is_external_cmd = [](const std::shared_ptr<lsh::assembler::cmd> &cmd) {
        return !is_own_cmd(cmd);
    };

    std::for_each(cmds.begin(), cmds.end(), [](const std::shared_ptr<lsh::assembler::cmd> &cmd) {
        if (is_own_cmd(cmd)) {
            handle_own_cmd(cmd);
        }
    });
    std::vector<std::shared_ptr<lsh::assembler::cmd>> extern_cmds;
    std::copy_if(cmds.begin(), cmds.end(), std::back_inserter(extern_cmds), copy_if_is_external_cmd);
    return handle_extern_cmds(extern_cmds);
}

bool is_own_cmd(const std::shared_ptr<lsh::assembler::cmd> &cmd) {
    std::vector<std::string> own_cmds = {
            "pwd", "cd"};
    return std::find(own_cmds.begin(), own_cmds.end(), cmd->name) != own_cmds.end();
}

void handle_own_cmd(const std::shared_ptr<lsh::assembler::cmd> &cmd) {
    if (cmd->name == "pwd") {
        lsh::cmd::handle_pwd(cmd);
    } else if (cmd->name == "cd") {
        lsh::cmd::handle_cd(cmd);
    }
}

std::string handle_extern_cmds(const std::vector<std::shared_ptr<lsh::assembler::cmd>> &cmds) {
    if (cmds.size() == 1) {
        int fd[2];
        auto cmd_to_exec = cmds.front();
        int child_status;
        const char *c = cmd_to_exec->name.c_str();
        char *args[cmd_to_exec->args.size() + 2];// = {const_cast<char *>(c), const_cast<char *>(latr), NULL};
        args[0] = const_cast<char *>(c);
        int i = 1;
        for (auto &arg : cmd_to_exec->args) {
            args[i] = const_cast<char *>(arg.c_str());
            i++;
        }
        args[i] = NULL;
        if (pipe(fd) == -1) {
            return "";
        }
        pid_t p;
        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "fork failed" << std::endl;
        } else if (pid == 0) {
            if (cmd_to_exec->name != "clear") {
                close(fd[0]);
                dup2(fd[1], 1);
                dup2(fd[1], 2);
                close(fd[1]);
            }
            execvp(c, args);
            std::cerr << "exec failed" << std::endl;
        } else {
            do {
                p = wait(&child_status);
            } while (p != pid);
            if (cmd_to_exec->name != "clear") {
                char buffer[2048];
                close(fd[1]);
                while (read(fd[0], buffer, sizeof(buffer)) != 0)
                    ;
                std::string data(buffer);
                return data;
            }
        }
    } else {
        std::string commands;
        std::for_each(cmds.begin(), cmds.end(), [&](const std::shared_ptr<lsh::assembler::cmd> &cmd) {
            std::string command(cmd->name);
            command += " ";
            std::for_each(cmd->args.begin(), cmd->args.end(), [&](const std::string &arg) {
                command = command + " " + arg;
            });
            if (!commands.empty())
                commands = commands + "|" + command;
            else
                commands = command;
        });
        piped_executor executor(cmds);
        return executor.execute();
    }
    return "";
}
