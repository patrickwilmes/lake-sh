#include "WorkingDirectory.hpp"
#include <filesystem>

LakeShell::WorkingDirectory::WorkingDirectory(std::string wd)
{
    refresh_wd(std::move(wd));
}

std::string& LakeShell::WorkingDirectory::get_working_dir()
{
    return m_current_wd;
}

bool LakeShell::WorkingDirectory::is_git_dir() const
{
    return m_is_git_dir;
}

void LakeShell::WorkingDirectory::refresh(std::string wd)
{
    refresh_wd(std::move(wd));
}

void LakeShell::WorkingDirectory::refresh_wd(std::string wd)
{
    m_current_wd = std::move(wd);
    m_is_git_dir = std::filesystem::exists(".git");
}
