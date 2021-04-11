#include "cmd.hpp"
#include "usr.hpp"
#include <iostream>

void lsh::cmd::handle_pwd(lsh::assembler::cmd *cmd) {
    char * wd = lsh::usr::current_wd();
    std::cout << wd << '\n' << std::flush;
    free(wd);
}
