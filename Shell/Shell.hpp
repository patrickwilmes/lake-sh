#ifndef LAKE_SH_LSH_H
#define LAKE_SH_LSH_H

#include "Alias.hpp"
#include "CmdHandler.hpp"
#include "ShellContext.hpp"
#include <csetjmp>
#include <csignal>
#include <memory>
#include "Prompt.hpp"

namespace LakeShell {
    using namespace LakeShell::Cmd;

    class Shell final {
    public:
        Shell();
        void run();
        static void sigint_handler();

    private:
        static void set_jump_active();
        static void setup_sig_handling();
    private:
        static sigjmp_buf m_env;
        static volatile sig_atomic_t m_jump_active;
        std::shared_ptr<ShellContext> m_shell_context;
        CommandHandler m_cmd_handler;
        Prompt m_prompt;
    };
} // namespace LakeShell

#endif //LAKE_SH_LSH_H
