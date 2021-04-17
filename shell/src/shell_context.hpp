#ifndef LAKE_SH_SHELL_CONTEXT_HPP
#define LAKE_SH_SHELL_CONTEXT_HPP

#include "alias.hpp"
#include <string>
#include <vector>

namespace lsh {
    class shell_context final {
    public:
        void refresh();
        std::string get_working_dir();
        std::string get_user_home();
        std::string get_username();
        std::string get_relative_working_dir();
        bool is_git_dir();

        void add_alias(std::string name, std::string origin);
        bool alias_exists(std::string &name);
        std::string get_origin_of_alias(std::string &name);
        void load_shell_profile();
    private:
        static const std::string LAKE_SHELL_PROFILE;
        bool m_is_git = false;
        std::string m_current_wd;
        std::string m_user_home;
        std::string m_username;
        std::vector<std::string> m_current_dirs;
        alias_container m_alias_container;
    };
}// namespace lsh

#endif//LAKE_SH_SHELL_CONTEXT_HPP
