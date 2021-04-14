#include "shell_context.hpp"
#include "usr.hpp"
#include "utils/utils.hpp"

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
