#ifndef LAKE_SH_CMD_HPP
#define LAKE_SH_CMD_HPP

#include "cmd_assembler.hpp"

namespace lsh::cmd {
    void handle_pwd(lsh::assembler::cmd *cmd);
    void handle_cd(lsh::assembler::cmd *cmd);
}

#endif//LAKE_SH_CMD_HPP
