#ifndef LAKE_SH_SHELLCONTEXT_HPP
#define LAKE_SH_SHELLCONTEXT_HPP

#include "Alias.hpp"
#include "fs/WorkingDirectory.hpp"
#include <memory>
#include <string>
#include <vector>
#include <ShellConfig.h>

namespace LakeShell {
    class ShellContext final {
    public:
        ShellContext();
        void refresh();
        std::string get_working_dir();
        std::string get_user_home();
        std::string get_username();
        bool is_git_dir();

        void add_alias(std::string name, std::string origin);
        bool alias_exists(std::string& name);
        std::string get_origin_of_alias(std::string& name);
        void load_shell_profile();
        std::string get_config(const std::string& key) const;

    private:
        static const std::string LAKE_SHELL_PROFILE;
        bool m_is_git = false;
        std::unique_ptr<WorkingDirectory> m_working_directory = nullptr;
        std::string m_user_home;
        std::string m_username;
        std::vector<std::string> m_current_dirs;
        AliasContainer m_alias_container;
        std::unique_ptr<LibConfig::ShellConfig> m_shell_config = nullptr;
    };
} // namespace LakeShell

#endif // LAKE_SH_SHELLCONTEXT_HPP
