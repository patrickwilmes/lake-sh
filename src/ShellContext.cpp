#include "ShellContext.hpp"

#include "User.hpp"
#include "utils/Utils.hpp"
#include "Parser.hpp"
#include <filesystem>
#include <fstream>
#include <utility>

using namespace LakeShell;
using namespace LakeShell::Parser;

const std::string ShellContext::LAKE_SHELL_PROFILE = ".lakesh";

void LakeShell::ShellContext::refresh()
{
    m_current_wd = LakeShell::User::current_wd();
    m_user_home = LakeShell::User::usr_home_dir();
    m_username = LakeShell::User::get_user_name();

    m_current_dirs = get_dirs_for(m_current_wd);

    m_is_git = std::filesystem::exists(".git");
    std::filesystem::path path = m_current_wd;
    if (!m_is_git) {
        while (path != m_user_home) {
            auto parent_git_path = path.append(".git");
            if (std::filesystem::exists(parent_git_path)) {
                m_is_git = true;
                break;
            }
            path = path.parent_path().parent_path();
        }
    }
}

std::string LakeShell::ShellContext::get_working_dir()
{
    return m_current_wd;
}

std::string LakeShell::ShellContext::get_user_home()
{
    return m_user_home;
}

std::string LakeShell::ShellContext::get_username()
{
    return m_username;
}

std::string LakeShell::ShellContext::get_relative_working_dir()
{
    return std::string();
}

bool LakeShell::ShellContext::is_git_dir()
{
    return m_is_git;
}

void LakeShell::ShellContext::add_alias(std::string name, std::string origin)
{
    m_alias_container.add(std::move(name), std::move(origin));
}

bool LakeShell::ShellContext::alias_exists(std::string& name)
{
    return m_alias_container.exists(name);
}

std::string LakeShell::ShellContext::get_origin_of_alias(std::string& name)
{
    return m_alias_container.get_origin(name);
}

void LakeShell::ShellContext::load_shell_profile()
{
    auto user_home = get_user_home();
    auto profile = user_home + "/" + LAKE_SHELL_PROFILE;

#ifdef SHELL_DEBUG
    auto alternative_profile = get_working_dir() + "/" + LAKE_SHELL_PROFILE;
    if (std::filesystem::exists(alternative_profile))
        profile = alternative_profile;
#endif

    if (!std::filesystem::exists(profile)) {
        return;
    }
    std::ifstream infile(profile);
    std::string line;
    while (std::getline(infile, line)) {
        auto cmds = parse_input(line);
        for (auto& cmd : cmds) {
            if (cmd->get_name() == "alias") {
                cmd->ensure_has_args(2);
                auto alias = cmd->get_args()[0];
                auto origin = cmd->get_args()[1];
                add_alias(alias, origin);
            }
        }
    }
}

ShellContext::ShellContext()
{
    load_shell_profile();
}

std::vector<std::string> ShellContext::get_path_completions(const std::string& partial_path)
{
    auto wd = get_working_dir();
    std::vector<std::string> path_completions;
    for (auto& path : m_current_dirs) {
        if (path.starts_with(partial_path)) {
            path_completions.push_back(path);
        }
    }
    return path_completions;
}
