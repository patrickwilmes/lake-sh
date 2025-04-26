#include "ShellContext.hpp"

#include "Parser.hpp"
#include "User.h"
#include <filesystem>
#include <fstream>
#include <utility>

using namespace LakeShell;
using namespace LakeShell::Parser;

const std::string ShellContext::LAKE_SHELL_PROFILE = ".lshrc";

void LakeShell::ShellContext::refresh()
{
    if (m_working_directory == nullptr)
        m_working_directory = std::make_unique<WorkingDirectory>(AK::current_wd());
    else
        m_working_directory->refresh(AK::current_wd());
    m_user_home = AK::usr_home_dir();
    m_username = AK::get_user_name();

    std::filesystem::path path = m_working_directory->get_working_dir();
    if (!m_is_git) {
        while (path != m_user_home && path != "/") {
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
    return m_working_directory->get_working_dir();
}

std::string LakeShell::ShellContext::get_user_home()
{
    return m_user_home;
}

std::string LakeShell::ShellContext::get_username()
{
    return m_username;
}

bool LakeShell::ShellContext::is_git_dir()
{
    return m_working_directory->is_git_dir();
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
        for (auto& cmd : cmds->get_commands()) {
            if (cmd->get_name() == "alias") {
                cmd->ensure_has_args(2);
                auto alias = cmd->get_args()[0];
                auto origin = cmd->get_args()[1];
                if (origin.starts_with("="))
                    origin.erase(0, 1);
                add_alias(alias, origin);
            }
        }
    }
}

ShellContext::ShellContext()
{
    load_shell_profile();
}
