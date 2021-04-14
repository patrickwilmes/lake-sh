#ifndef LAKE_SH_USR_HPP
#define LAKE_SH_USR_HPP

#include <string>

namespace lsh::usr {

    constexpr int MAX_USERNAME_LEN = 32;

    std::string get_user_name();

    std::string current_wd();

    std::string usr_home_dir();
}

#endif//LAKE_SH_USR_HPP
