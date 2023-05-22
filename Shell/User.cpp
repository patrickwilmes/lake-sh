#include "User.hpp"
#include <climits>
#include <pwd.h>
#include <unistd.h>

std::string LakeShell::User::get_user_name()
{
#ifndef WSL_COMPATIBILITY_MODE
    // GitHub Issue regarding WSL getlogin issue https://github.com/microsoft/WSL/issues/888
    char* user = getlogin();
    return { user };
#else
    return "wsl";
#endif
}

std::string LakeShell::User::current_wd()
{
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        return { cwd };
    }
    return "";
}

std::string LakeShell::User::usr_home_dir()
{
    struct passwd* pw = getpwuid(getuid());
    return { pw->pw_dir };
}
