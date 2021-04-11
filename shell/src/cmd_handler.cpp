#include "cmd_handler.hpp"
#include <string>

bool is_own_cmd(lsh::assembler::cmd *cmd);
void handle_own_cmd(lsh::assembler::cmd *cmd);
void handle_cmd(lsh::assembler::cmd *cmd);

void lsh::cmd::handle_commands(std::vector<lsh::assembler::cmd *> cmds) {
    std::for_each(cmds.begin(), cmds.end(), [](lsh::assembler::cmd *cmd){
        if (is_own_cmd(cmd)) {
            handle_own_cmd(cmd);
        } else {
            handle_cmd(cmd);
        }
    });
}

bool is_own_cmd(lsh::assembler::cmd *cmd) {
    std::vector<std::string> own_cmds = {
            "pwd", "cd"
    };
    return std::find(own_cmds.begin(), own_cmds.end(), cmd->name) != own_cmds.end();
}

void handle_own_cmd(lsh::assembler::cmd *cmd) {

}

void handle_cmd(lsh::assembler::cmd *cmd) {

}
