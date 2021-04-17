#ifndef LAKE_SH_LSH_H
#define LAKE_SH_LSH_H

#include "alias.hpp"
#include "history.hpp"
#include "shell_context.hpp"
#include "cmd_handler.hpp"
#include <terminal.hpp>
#include <ncurses.h>
#include <memory>

namespace lsh {
    using namespace lsh::cmd;

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
        std::shared_ptr<shell_context> m_shell_context;
        history m_history;
        command_handler m_cmd_handler;
        term::terminal m_term;
    };
}// namespace lsh

#endif//LAKE_SH_LSH_H
