#ifndef LAKE_SH_LOGGER_HPP
#define LAKE_SH_LOGGER_HPP

#include <string>
#include <ncurses.h>

namespace lsh {
    class logger final {
    public:
        explicit logger(WINDOW *win);
        void log_info(std::string msg);
    private:
        WINDOW *m_win;
    };
}

#endif//LAKE_SH_LOGGER_HPP
