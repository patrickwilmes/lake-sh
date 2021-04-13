#include "cmd_handler.hpp"
#include "cmd/cmd.hpp"
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <utility>
#include <ncurses.h>

bool is_own_cmd(const std::shared_ptr<lsh::assembler::cmd> &cmd);
void handle_own_cmd(const std::shared_ptr<lsh::assembler::cmd> &cmd);
std::string handle_extern_cmds(const std::vector<std::shared_ptr<lsh::assembler::cmd>> &cmds);

class piped_executor final {
public:
    explicit piped_executor(std::vector<std::shared_ptr<lsh::assembler::cmd>> cmds) : m_cmds(std::move(cmds)) {}

    void execute() {
        int pid;
        int pipe1[2];
        int pipe2[2];
        bool is_pipe1_write = true;
        uint32_t counter = 0;
        std::for_each(m_cmds.begin(), m_cmds.end(), [&](const std::shared_ptr<lsh::assembler::cmd> &cmd) {
            if (pipe(pipe1) == -1) {
                std::cerr << "failed pipe" << std::endl;
            }
            ++counter;
        });
    }

private:
    void execute_front_piped(const std::shared_ptr<lsh::assembler::cmd> &cmd) {
    }
    void execute_mid_piped(const std::shared_ptr<lsh::assembler::cmd> &cmd) {
    }
    void execute_back_piped(const std::shared_ptr<lsh::assembler::cmd> &cmd) {
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
        if(pipe(fd) == -1) {
            return "";
        }
        pid_t p;
        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "fork failed" << std::endl;
        } else if (pid == 0) {
            close(fd[0]);
            dup2(fd[1], 1);
            dup2(fd[1], 2);
            close(fd[1]);
            execvp(c, args);
            std::cerr << "exec failed" << std::endl;
        } else {
            do {
                p = wait(&child_status);
            } while (p != pid);
            char buffer[2048];
            close(fd[1]);
            while (read(fd[0], buffer, sizeof(buffer)) != 0);
            std::string data(buffer);
            return data;
        }
    } else {
        std::string commands;
        std::for_each(cmds.begin(), cmds.end(), [&](const std::shared_ptr<lsh::assembler::cmd> &cmd) {
            std::string command(cmd->name);
            command += " ";
            std::for_each(cmd->args.begin(), cmd->args.end(), [&](const std::string &arg){
                command = command + " " + arg;
            });
            if (!commands.empty())
                commands = commands + "|" + command;
            else
                commands = command;
        });
        system(commands.c_str());
//        piped_executor executor(cmds);
//        executor.execute();
    }
    return "";
}
