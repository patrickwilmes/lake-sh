#include "shell_context.hpp"

#include <utility>
#include "usr.hpp"
#include "utils/utils.hpp"

using namespace lsh;

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
    printf("adding alias %s %s", name.c_str(), origin.c_str());
    m_alias_container.add(std::move(name), std::move(origin));
}

bool lsh::shell_context::alias_exists(std::string &name) {
    return m_alias_container.exists(name);
}

std::string lsh::shell_context::get_origin_of_alias(std::string &name) {
    return m_alias_container.get_origin(name);
}
