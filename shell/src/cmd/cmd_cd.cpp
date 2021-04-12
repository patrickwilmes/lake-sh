#include "cmd.hpp"
#include "usr.hpp"
#include <cassert>
#include <string>
#include <unistd.h>

std::string resolve_path(std::string &origin_path) {
    if (origin_path == "..") {
        return origin_path.substr(0, origin_path.rfind('/') - 1);
    } else if ((origin_path[0] != '.' && origin_path[0] != '/') || (origin_path[0] == '.' && origin_path[1] == '/')) {
        char *wd = lsh::usr::current_wd();
        std::string wds(wd);
        free(wd);
        return wds + "/" + origin_path;
    }
    assert(origin_path[0] == '/');
    return origin_path;
}

void lsh::cmd::handle_cd(std::shared_ptr<lsh::assembler::cmd> cmd) {
    if (cmd->args.empty()) {
        char *usr_home = lsh::usr::usr_home_dir();
        chdir(usr_home);
        free(usr_home);
    } else {
        chdir(resolve_path(cmd->args[0]).c_str());
    }
}
