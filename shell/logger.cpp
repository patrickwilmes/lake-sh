#include "logger.hpp"

void lsh::logger::log_info(const std::string msg) {
#ifdef SHELL_DEBUG
    mvwaddstr(m_win, 10, 10, msg.c_str());
#endif
}

lsh::logger::logger(WINDOW *win): m_win(win) {
}
