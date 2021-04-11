#include "cmd_handler.hpp"
#include <algorithm>

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
