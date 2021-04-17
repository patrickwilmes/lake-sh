#include "shell_context.hpp"

#include "usr.hpp"
#include "utils/utils.hpp"
#include "tokenizer.hpp"
#include "cmd_assembler.hpp"
#include <utility>
#include <filesystem>
#include <fstream>

using namespace lsh;
using namespace lsh::tokenizer;
using namespace lsh::assembler;

const std::string shell_context::LAKE_SHELL_PROFILE = ".lakesh";

void lsh::shell_context::refresh() {
    m_current_wd = lsh::usr::current_wd();
    m_user_home = lsh::usr::usr_home_dir();
    m_username = lsh::usr::get_user_name();

    m_current_dirs = get_dirs_for(m_current_wd);

    m_is_git = std::filesystem::exists(".git");
}

std::string lsh::shell_context::get_working_dir() {
    return m_current_wd;
}

std::string lsh::shell_context::get_user_home() {
    return m_user_home;
}

std::string lsh::shell_context::get_username() {
    return m_username;
}

std::string lsh::shell_context::get_relative_working_dir() {
    return std::string();
}

bool lsh::shell_context::is_git_dir() {
    return m_is_git;
}

void lsh::shell_context::add_alias(std::string name, std::string origin) {
    m_alias_container.add(std::move(name), std::move(origin));
}

bool lsh::shell_context::alias_exists(std::string &name) {
    return m_alias_container.exists(name);
}

std::string lsh::shell_context::get_origin_of_alias(std::string &name) {
    return m_alias_container.get_origin(name);
}

void lsh::shell_context::load_shell_profile() {
    auto user_home = get_user_home();
    auto profile = user_home + "/" + LAKE_SHELL_PROFILE;
    if (!std::filesystem::exists(profile)) {
        return;
    }
    std::ifstream infile(profile);
    std::string line;
    while (std::getline(infile, line)) {
        auto tokens = tokenize(line);
        auto cmds = assemble_commands(tokens);
        for (auto &cmd : cmds) {
            if (cmd->get_name() == "alias") {
                cmd->ensure_has_args(2);
                auto alias = cmd->get_args()[0];
                auto origin = cmd->get_args()[1];
                add_alias(alias, origin);
            }
        }
    }
}

shell_context::shell_context() {
    load_shell_profile();
}
