#include "User.hpp"
#include <climits>
#include <pwd.h>
#include <unistd.h>

std::string LakeShell::User::get_user_name() {
    char *user = getlogin();
    return std::string(user);
}

std::string LakeShell::User::current_wd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        return std::string(cwd);
    }
    return "";
}

std::string LakeShell::User::usr_home_dir() {
    struct passwd *pw = getpwuid(getuid());
    return std::string(pw->pw_dir);
}
