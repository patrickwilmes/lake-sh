#ifndef LAKE_SH_USER_HPP
#define LAKE_SH_USER_HPP

#include <string>

namespace LakeShell::User {

    constexpr int MAX_USERNAME_LEN = 32;

    std::string get_user_name();

    std::string current_wd();

    std::string usr_home_dir();
}

#endif //LAKE_SH_USER_HPP
