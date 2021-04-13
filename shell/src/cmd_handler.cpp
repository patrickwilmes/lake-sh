#include "cmd_handler.hpp"
#include "cmd/cmd.hpp"
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>

bool is_own_cmd(std::shared_ptr<lsh::assembler::cmd> cmd);
void handle_own_cmd(std::shared_ptr<lsh::assembler::cmd> cmd);
void handle_extern_cmds(std::vector<std::shared_ptr<lsh::assembler::cmd>> &cmds);

void lsh::cmd::handle_commands(std::vector<std::shared_ptr<lsh::assembler::cmd>> cmds) {
    auto copy_if_is_external_cmd = [](std::shared_ptr<lsh::assembler::cmd> cmd) {
        return !is_own_cmd(cmd);
    };

    std::for_each(cmds.begin(), cmds.end(), [](std::shared_ptr<lsh::assembler::cmd> cmd) {
        if (is_own_cmd(cmd)) {
            handle_own_cmd(cmd);
        }
    });
    std::vector<std::shared_ptr<lsh::assembler::cmd>> extern_cmds;
    std::copy_if(cmds.begin(), cmds.end(), std::back_inserter(extern_cmds), copy_if_is_external_cmd);
    handle_extern_cmds(extern_cmds);
}

bool is_own_cmd(std::shared_ptr<lsh::assembler::cmd> cmd) {
    std::vector<std::string> own_cmds = {
            "pwd", "cd"};
    return std::find(own_cmds.begin(), own_cmds.end(), cmd->name) != own_cmds.end();
}

void handle_own_cmd(std::shared_ptr<lsh::assembler::cmd> cmd) {
    if (cmd->name == "pwd") {
        lsh::cmd::handle_pwd(cmd);
    } else if (cmd->name == "cd") {
        lsh::cmd::handle_cd(cmd);
    }
}

void handle_extern_cmds(std::vector<std::shared_ptr<lsh::assembler::cmd>> &cmds) {
    if (cmds.size() == 1) {
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
        pid_t p;
        pid_t pid = fork();
        if (pid < 0) {
            std::cerr << "fork failed" << std::endl;
        } else if (pid == 0) {
            execvp(c, args);
            std::cerr << "exec failed" << std::endl;
        } else {
            do {
                p = wait(&child_status);
            } while (p != pid);
        }
    }
}
