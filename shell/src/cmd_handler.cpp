#include "cmd_handler.hpp"
#include "cmd/cmd.hpp"
#include <string>

#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

bool is_piped(std::vector<lsh::assembler::cmd *> cmds);

void exec_single_command(lsh::assembler::cmd * cmd);

void exec_first_cmd(int pid, int *p1, int *p2);
void exec_middle_cmd(int pid, int *p1, int *p2);
void exec_last_cmd(int pid, int *p1, int *p2);

bool is_own_cmd(lsh::assembler::cmd *cmd);
void handle_own_cmd(lsh::assembler::cmd *cmd);
void handle_extern_cmds(std::vector<lsh::assembler::cmd *> &cmds);

void lsh::cmd::handle_commands(std::vector<lsh::assembler::cmd *> cmds) {
    auto copy_if_is_external_cmd = [](lsh::assembler::cmd *cmd) {
      return !is_own_cmd(cmd);
    };

    std::for_each(cmds.begin(), cmds.end(), [](lsh::assembler::cmd *cmd) {
        if (is_own_cmd(cmd)) {
            handle_own_cmd(cmd);
        }
    });
    std::vector<lsh::assembler::cmd *> extern_cmds;
    std::copy_if(cmds.begin(), cmds.end(), std::back_inserter(extern_cmds), copy_if_is_external_cmd);
    handle_extern_cmds(extern_cmds);
}

bool is_own_cmd(lsh::assembler::cmd *cmd) {
    std::vector<std::string> own_cmds = {
            "pwd", "cd"};
    return std::find(own_cmds.begin(), own_cmds.end(), cmd->name) != own_cmds.end();
}

void handle_own_cmd(lsh::assembler::cmd *cmd) {
    if (cmd->name == "pwd") {
        lsh::cmd::handle_pwd(cmd);
    } else if (cmd->name == "cd") {
        lsh::cmd::handle_cd(cmd);
    }
}

void handle_extern_cmds(std::vector<lsh::assembler::cmd *> &cmds) {
    if (is_piped(cmds)) {
        // todo - exec piped
    } else {
        exec_single_command(cmds.front());
    }
}

bool is_piped(std::vector<lsh::assembler::cmd *> cmds) {
    return cmds.size() > 1;
}

void exec_single_command(lsh::assembler::cmd * cmd) {
    const char *command_name = cmd->name.c_str();
    std::vector<const char *> cstrings;
    for (int i = 0; i < cmd->args.size(); i++) {
        auto arg = cmd->args[i]->name;
        const char *carg = arg.c_str();
        cstrings.emplace_back(carg);
    }
    execlp(command_name, command_name, &cstrings[0], NULL);
}
