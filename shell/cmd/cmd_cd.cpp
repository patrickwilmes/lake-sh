#include "../usr.hpp"
#include "cmd.hpp"
#include <cassert>
#include <string>
#include <unistd.h>

std::string resolve_path(std::string &origin_path) {
    if (origin_path == "..") {
        return origin_path.substr(0, origin_path.rfind('/') - 1);
    } else if ((origin_path[0] != '.' && origin_path[0] != '/') || (origin_path[0] == '.' && origin_path[1] == '/')) {
        std::string wd = lsh::usr::current_wd();
        return wd + "/" + origin_path;
    }
    assert(origin_path[0] == '/');
    return origin_path;
}

void lsh::cmd::handle_cd(const std::shared_ptr<lsh::assembler::cmd>& cmd) {
    if (cmd->args.empty()) {
        std::string usr_home = lsh::usr::usr_home_dir();
        chdir(usr_home.c_str());
    } else {
        chdir(resolve_path(cmd->args[0]).c_str());
    }
}
