#include "shell_context.hpp"
#include "usr.hpp"
#include "utils/utils.hpp"

void lsh::shell_context::refresh() {
    char *wd_cstr = lsh::usr::current_wd();
    char *home_cstr = lsh::usr::usr_home_dir();
    char *username_cstr = lsh::usr::get_user_name();

    std::string wd(wd_cstr);
    std::string home(home_cstr);
    std::string username(username_cstr);

    m_current_wd = wd;
    m_user_home = home;
    m_username = username;

    m_current_dirs = get_dirs_for(wd);

    if (std::find(m_current_dirs.begin(), m_current_dirs.end(), ".git") != m_current_dirs.end()) {
        m_is_git = true;
    }

    free(wd_cstr);
    free(home_cstr);
    free(username_cstr);
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
