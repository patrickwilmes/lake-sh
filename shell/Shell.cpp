#include "Shell.hpp"
#include "utils/Utils.hpp"
#include "Cmd.hpp"
#include "CmdAssembler.hpp"
#include "CmdHandler.hpp"
#include "Tokenizer.hpp"
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace LakeShell;

static const std::string EXIT_KWD = "exit";
constexpr int CTRL_L_KEY = 12;
constexpr int ENTER_KEY = 10;

constexpr int HISTORY_SIZE = 256;

volatile sig_atomic_t Shell::m_jump_active = 0;
sigjmp_buf Shell::m_env;

std::vector<std::shared_ptr<LakeShell::Cmd::Command>> process_input(std::string& line);

Shell::Shell()
    : m_shell_context(std::make_shared<ShellContext>())
    , m_history(History(HISTORY_SIZE))
    , m_cmd_handler(LakeShell::Cmd::CommandHandler(m_shell_context))
{
    m_shell_context->refresh();
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

        auto p = prompt();
        char * line = readline(p.c_str());
        std::string input(line);
        if (input == "exit") {
            break;
        }

        trim(input);
        if (!input.empty()) {
            auto tokens = LakeShell::Tokenizer::tokenize(input);
            auto cmds = LakeShell::Assembler::assemble_commands(tokens);
            try {
                m_cmd_handler.handle_commands(cmds);
                add_history(line);
            }  catch (LakeShell::Cmd::CommandNotFoundException &e) {
                std::cout << "command not found: " << e.what() << '\n' << std::flush;
            } catch (LakeShell::Cmd::InvalidCommandException &e) {
                std::cout << "invalid command call: " << e.what() << '\n' << std::flush;
            } catch (std::exception &e) {
                std::cout << "something went wrong: " << e.what() << std::flush;
            }
        }
    }
}

void Shell::set_jump_active()
{
    m_jump_active = 1;
}

void Shell::sigint_handler(int signo)
{
    if (!m_jump_active) {
        return;
    }
    siglongjmp(m_env, 42);
}

std::string Shell::prompt()
{
    std::string prompt = m_shell_context->get_username();
    auto user_home = m_shell_context->get_user_home();
    auto wd = m_shell_context->get_working_dir();
    if (wd.length() > user_home.length()) {
        prompt += " @ ~" + wd.substr(user_home.length(), wd.length() - 1);
    } else {
        prompt += " @ " + m_shell_context->get_working_dir();
    }
    return prompt + " >> ";
}

void Shell::setup_sig_handling()
{
    struct sigaction s{};
    s.sa_handler = Shell::sigint_handler;
    sigemptyset(&s.sa_mask);
    s.sa_flags = SA_RESTART;
    sigaction(SIGINT, &s, nullptr);
}

std::vector<std::shared_ptr<LakeShell::Cmd::Command>> process_input(std::string& line)
{
    auto tokens = LakeShell::Tokenizer::tokenize(line);
    return LakeShell::Assembler::assemble_commands(tokens);
}
