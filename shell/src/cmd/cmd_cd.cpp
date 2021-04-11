#include "usr.hpp"
#include "cmd.hpp"
#include <unistd.h>

void lsh::cmd::handle_cd(lsh::assembler::cmd *cmd) {
    if (cmd->args.empty()) {
        char *usr_home = lsh::usr::usr_home_dir();
        chdir(usr_home);
        free(usr_home);
    }
}
