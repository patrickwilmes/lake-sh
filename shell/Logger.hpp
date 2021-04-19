#ifndef LAKE_SH_LOGGER_HPP
#define LAKE_SH_LOGGER_HPP

#include <iostream>
#include <ncurses.h>
#include <string>

namespace LakeShell {
    template<typename T>
    void dbg(T t)
    {
#ifdef SHELL_DEBUG
        std::cout << t << std::endl;
#endif
    }

    template<typename T, typename Arg>
    void dbg(T t, Arg a)
    {
#ifdef SHELL_DEBUG
        std::cout << t << " " << a << std::endl;
#endif
    }

    template<typename T, typename ArgOne, typename ArgTwo>
    void dbg(T t, ArgOne a, ArgTwo b)
    {
#ifdef SHELL_DEBUG
        std::cout << t << " " << a << " " << b << std::endl;
#endif
    }
} // namespace LakeShell

#endif //LAKE_SH_LOGGER_HPP
