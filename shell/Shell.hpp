#ifndef LAKE_SH_LSH_H
#define LAKE_SH_LSH_H

#include "Alias.hpp"
#include "CmdHandler.hpp"
#include "History.hpp"
#include "ShellContext.hpp"
#include <Terminal.hpp>
#include <memory>
#include <ncurses.h>

namespace LakeShell {
    using namespace LakeShell::Cmd;

    class Shell final {
    public:
        Shell();
        ~Shell();
        void run();

    private:
        void display_prompt();

    private:
        WINDOW *m_win = nullptr;
        bool m_running = true;
        std::shared_ptr<ShellContext> m_shell_context;
        History m_history;
        CommandHandler m_cmd_handler;
        Term::Terminal m_term;
    };
}// namespace LakeShell

#endif//LAKE_SH_LSH_H
