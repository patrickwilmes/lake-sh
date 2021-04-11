#ifndef LAKE_SH_CMD_HANDLER_HPP
#define LAKE_SH_CMD_HANDLER_HPP

#include "cmd_assembler.hpp"
#include <vector>

namespace lsh::cmd {
   void handle_commands(std::vector<lsh::assembler::cmd *> cmds);
}

#endif//LAKE_SH_CMD_HANDLER_HPP
