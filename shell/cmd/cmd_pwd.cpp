#include "../usr.hpp"
#include "cmd.hpp"
#include <iostream>
#include <string>

void lsh::cmd::handle_pwd(std::shared_ptr<lsh::assembler::cmd> cmd) {
    std::string wd = lsh::usr::current_wd();
    std::cout << wd << '\n' << std::flush;
}
