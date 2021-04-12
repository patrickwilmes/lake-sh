#ifndef LAKE_SH_CMD_HPP
#define LAKE_SH_CMD_HPP

#include "cmd_assembler.hpp"
#include <memory>

namespace lsh::cmd {
    void handle_pwd(std::shared_ptr<lsh::assembler::cmd> cmd);
    void handle_cd(std::shared_ptr<lsh::assembler::cmd> cmd);
}

#endif//LAKE_SH_CMD_HPP
