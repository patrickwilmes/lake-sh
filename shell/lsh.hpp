#ifndef LAKE_SH_LSH_H
#define LAKE_SH_LSH_H

#include "history.hpp"
#include "shell_context.hpp"
#include <ncurses.h>

namespace lsh {
    class lake_shell final {
    public:
        lake_shell();
        ~lake_shell();
        void run();

    private:
        void display_prompt();
    private:
        WINDOW *m_win = nullptr;
        bool m_running = true;
        shell_context m_shell_context;
        history m_history;
    };
}// namespace lsh

#endif//LAKE_SH_LSH_H
