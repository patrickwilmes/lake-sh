#include "logger.hpp"

void lsh::logger::log_info(const std::string msg) {
#ifdef SHELL_DEBUG
    int y, x;
    getyx(m_win, y, x);
    mvwaddstr(m_win, 20, 0, msg.c_str());
    move(y, x);
    refresh();
#endif
}

lsh::logger::logger(WINDOW *win): m_win(win) {
}
