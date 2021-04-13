#include "../usr.hpp"
#include "cmd.hpp"
#include <iostream>

void lsh::cmd::handle_pwd(std::shared_ptr<lsh::assembler::cmd> cmd) {
    char * wd = lsh::usr::current_wd();
    std::cout << wd << '\n' << std::flush;
    free(wd);
}
