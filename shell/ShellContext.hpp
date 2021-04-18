#ifndef LAKE_SH_SHELLCONTEXT_HPP
#define LAKE_SH_SHELLCONTEXT_HPP

#include "Alias.hpp"
#include <string>
#include <vector>

namespace LakeShell {
    class ShellContext final {
    public:
        ShellContext();
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
        std::vector<std::string> get_path_completions(const std::string &partial_path);
    private:
        static const std::string LAKE_SHELL_PROFILE;
        bool m_is_git = false;
        std::string m_current_wd;
        std::string m_user_home;
        std::string m_username;
        std::vector<std::string> m_current_dirs;
        AliasContainer m_alias_container;
    };
}// namespace LakeShell

#endif //LAKE_SH_SHELLCONTEXT_HPP