#include "cmd.hpp"
#include "usr.hpp"
#include <climits>
#include <cstring>
#include <string>
#include <unistd.h>

std::string resolve_path(std::string &origin_path) {
    if (origin_path == "..") {
        auto new_path = origin_path.substr(0, origin_path.rfind('/') - 1);
        chdir(new_path.c_str());
    }
}

void lsh::cmd::handle_cd(std::shared_ptr<lsh::assembler::cmd> cmd) {
    if (cmd->args.empty()) {
        char *usr_home = lsh::usr::usr_home_dir();
        chdir(usr_home);
        free(usr_home);
    } else {
        resolve_path(cmd->args[0]);
    }
}
