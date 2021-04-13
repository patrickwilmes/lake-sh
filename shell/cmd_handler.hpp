#ifndef LAKE_SH_CMD_HANDLER_HPP
#define LAKE_SH_CMD_HANDLER_HPP

#include "cmd_assembler.hpp"
#include <memory>
#include <vector>

namespace lsh::cmd {
    std::string handle_commands(std::vector<std::shared_ptr<lsh::assembler::cmd>> cmds);
}

#endif//LAKE_SH_CMD_HANDLER_HPP
