#include "usr.hpp"
#include "cmd.hpp"
#include <unistd.h>
#include <string>
#include <climits>
#include <cstring>

std::string resolve_path(std::string &origin_path) {
    if (origin_path == "..") {
        char *wd = lsh::usr::usr_home_dir();
        char *target_path = (char *) malloc(sizeof(char) * PATH_MAX);
        char *pch = strrchr(wd, '/');
//        strcpy(target_path, strncpy()
        free(wd);
    }
}

void lsh::cmd::handle_cd(lsh::assembler::cmd *cmd) {
    if (cmd->args.empty()) {
        char *usr_home = lsh::usr::usr_home_dir();
        chdir(usr_home);
        free(usr_home);
    } else {
        chdir(cmd->args[0]->value.c_str());
    }
}
