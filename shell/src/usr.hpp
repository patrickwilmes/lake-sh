#ifndef LAKE_SH_USR_HPP
#define LAKE_SH_USR_HPP

namespace lsh::usr {

    constexpr int MAX_USERNAME_LEN = 32;

    char * get_user_name();

    char * current_wd();

}

#endif//LAKE_SH_USR_HPP
