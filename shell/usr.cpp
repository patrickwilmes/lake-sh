#include "usr.hpp"
#include <unistd.h>
#include <climits>
#include <pwd.h>

std::string lsh::usr::get_user_name() {
    char *user = getlogin();
    return std::string(user);
}

std::string lsh::usr::current_wd() {
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        return std::string(cwd);
    }
    return "";
}

std::string lsh::usr::usr_home_dir() {
    struct passwd *pw = getpwuid(getuid());
    return std::string(pw->pw_dir);
}
