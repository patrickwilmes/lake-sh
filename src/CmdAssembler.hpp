#ifndef LAKE_SH_CMD_ASSEMBLER_H
#define LAKE_SH_CMD_ASSEMBLER_H

#include "Cmd.hpp"
#include <memory>
#include <string>
#include <vector>

namespace LakeShell::Assembler {
    std::vector<std::shared_ptr<LakeShell::Cmd::Command>> assemble_commands(std::vector<std::string> tokens);
} // namespace LakeShell::Assembler

#endif //LAKE_SH_CMD_ASSEMBLER_H
