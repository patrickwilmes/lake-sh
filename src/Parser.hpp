#ifndef LAKE_SH_PARSER_H
#define LAKE_SH_PARSER_H

#include "Cmd.hpp"
#include <memory>
#include <vector>

namespace LakeShell::Parser {
    std::vector<std::shared_ptr<LakeShell::Cmd::Command>> parse_input(std::string &input);
}

#endif