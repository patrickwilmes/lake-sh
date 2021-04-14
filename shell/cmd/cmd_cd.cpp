#include "../usr.hpp"
#include "cmd.hpp"
#include <cassert>
#include <string>
#include <unistd.h>

std::string resolve_path(std::string &origin_path) {
    std::string wd = lsh::usr::current_wd();
    if (origin_path == "..") {
        return wd.substr(0, wd.rfind('/'));
    } else if ((origin_path[0] != '.' && origin_path[0] != '/') || (origin_path[0] == '.' && origin_path[1] == '/')) {
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
        std::string path_string = resolve_path(cmd->args[0]);
        chdir(path_string.c_str());
    }
}
