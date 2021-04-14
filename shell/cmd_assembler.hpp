#ifndef LAKE_SH_CMD_ASSEMBLER_H
#define LAKE_SH_CMD_ASSEMBLER_H

#include "cmd.hpp"
#include <string>
#include <vector>
#include <memory>

namespace lsh::assembler {
    std::vector<std::shared_ptr<lsh::cmd::command>> assemble_commands(std::vector<std::string> tokens);
}// namespace lsh::assembler

#endif//LAKE_SH_CMD_ASSEMBLER_H
