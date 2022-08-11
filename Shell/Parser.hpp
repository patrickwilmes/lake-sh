#ifndef LAKE_SH_PARSER_H
#define LAKE_SH_PARSER_H

#include "Cmd.hpp"
#include <memory>
#include <vector>

namespace LakeShell::Parser {
    std::shared_ptr<LakeShell::Cmd::CommandContainer> parse_input(std::string &input);
}

#endif