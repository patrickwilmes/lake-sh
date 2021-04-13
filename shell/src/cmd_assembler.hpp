#ifndef LAKE_SH_CMD_ASSEMBLER_H
#define LAKE_SH_CMD_ASSEMBLER_H

#include <string>
#include <vector>
#include <memory>

namespace lsh::assembler {
    typedef struct {
        std::string name;
        std::vector<std::string> args;
    } cmd;

    std::vector<std::shared_ptr<cmd>> assemble_commands(std::vector<std::string> tokens);
    void free_commands(std::vector<cmd *> cmds);
}// namespace lsh::assembler

#endif//LAKE_SH_CMD_ASSEMBLER_H
