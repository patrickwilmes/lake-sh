#include "Shell.hpp"
#include "Cmd.hpp"
#include "CmdHandler.hpp"
#include "Parser.hpp"
#include "utils/Utils.hpp"
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>
#include <string>
#include <vector>

using namespace LakeShell;

volatile sig_atomic_t Shell::m_jump_active = 0;
sigjmp_buf Shell::m_env;

Shell::Shell()
    : m_shell_context(std::make_shared<ShellContext>())
    , m_cmd_handler(LakeShell::Cmd::CommandHandler(m_shell_context))
    , m_prompt(Prompt(m_shell_context))
{
    m_shell_context->refresh();
    m_shell_context->load_shell_profile();
}

void Shell::run()
{
    setup_sig_handling();
    while (true) {
        if (sigsetjmp(m_env, 1) == 42) {
            printf("\n");
            continue;
        }
        Shell::set_jump_active();

        auto p = m_prompt.render_prompt();
        char* line = readline(p.c_str());
        std::string input(line);
        if (input == "exit") {
            break;
        }

        trim(input);
        if (!input.empty()) {
            auto cmds = LakeShell::Parser::parse_input(input);
            try {
                m_cmd_handler.handle_commands(cmds);
                add_history(line);
                m_shell_context->refresh();
            } catch (LakeShell::Cmd::CommandNotFoundException& e) {
                std::cout << "command not found: " << e.what() << '\n'
                          << std::flush;
            } catch (LakeShell::Cmd::InvalidCommandException& e) {
                std::cout << "invalid command call: " << e.what() << '\n'
                          << std::flush;
            } catch (std::exception& e) {
                std::cout << "something went wrong: " << e.what() << std::flush;
            }
        }
    }
}

void Shell::set_jump_active()
{
    m_jump_active = 1;
}

void Shell::sigint_handler()
{
    if (!m_jump_active) {
        return;
    }
    siglongjmp(m_env, 42);
}

void Shell::setup_sig_handling()
{
    struct sigaction s {
    };
    s.sa_handler = reinterpret_cast<__sighandler_t>(Shell::sigint_handler);
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    sigaction(SIGINT, &s, nullptr);
}
