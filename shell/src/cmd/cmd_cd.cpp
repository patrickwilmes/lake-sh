#include "usr.hpp"
#include "cmd.hpp"
#include <unistd.h>
#include <string>

bool is_relative_path(std::string &path) {
    return false;
}

void lsh::cmd::handle_cd(lsh::assembler::cmd *cmd) {
    if (cmd->args.empty()) {
        char *usr_home = lsh::usr::usr_home_dir();
        chdir(usr_home);
        free(usr_home);
    }
}
