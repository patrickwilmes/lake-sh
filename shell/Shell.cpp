#include "Shell.hpp"
#include "Cmd.hpp"
#include "CmdAssembler.hpp"
#include "CmdHandler.hpp"
#include "Tokenizer.hpp"
#include "utils/Utils.hpp"
#include <memory>
#include <string>
#include <vector>

using namespace LakeShell;

static const std::string EXIT_KWD = "exit";
constexpr int CTRL_L_KEY = 12;
constexpr int ENTER_KEY = 10;

std::vector<std::shared_ptr<LakeShell::Cmd::Command>> process_input(std::string &line);

Shell::Shell() : m_shell_context(std::make_shared<ShellContext>()), m_history(History(256)), m_cmd_handler(LakeShell::Cmd::CommandHandler(m_shell_context)) {
    m_shell_context->refresh();
}

Shell::~Shell() {
    endwin();
}

void Shell::run() {
    display_prompt();
    while (m_running) {
        //        char *inbuf = (char *) malloc(sizeof(char) * 2048);
        bool collecting_input = true;
        bool direct_command = false;
        //        int x, y;
        //        getyx(m_win, y, x);
        auto input = m_term.get_input();
        std::string command_input(input.input);
        trim(command_input);
        //        free(inbuf);
        echo();
        refresh();
        if (command_input == EXIT_KWD) {
            m_running = false;
        } else if (!direct_command) {
            if (!command_input.empty()) {
                auto cmds = process_input(command_input);
                try {
                    std::string render_data = m_cmd_handler.handle_commands(cmds);
                    trim(render_data);
                    if (!render_data.empty()) {
                        std::stringstream ss(render_data);
                        std::string target;
                        int cy, cx;
                        getyx(m_win, cy, cx);
                        cy++;
                        while (std::getline(ss, target, '\n')) {
                            if (!target.empty()) {
                                mvaddstr(cy, 0, target.c_str());
                                cy++;
                            }
                        }
                    }
                    m_history.add(command_input);
                } catch (InvalidCommandException&e) {
                    m_term.print_next_line("invalid Command class: ");
                    m_term.print(e.what());
                } catch (CommandNotFoundException&e) {
                    m_term.print_next_line("Command not found: ");
                    m_term.print(e.what());
                } catch (std::runtime_error &e) {
                    m_term.print_next_line("Command not found: ");
                    m_term.print(e.what());
                } catch (std::exception &e) {
                    m_term.print_next_line("error: ");
                    m_term.print(e.what());
                }
            }
        }
        noecho();
        m_shell_context->refresh();
        display_prompt();
        refresh();
    }
}

void Shell::display_prompt() {
    int y, x;
    getyx(m_win, y, x);
    y++;
    echo();
    start_color();
    mvaddstr(y, 0, m_shell_context->get_username().c_str());
    addstr(" @ ");
    auto user_home = m_shell_context->get_user_home();
    auto working_dir = m_shell_context->get_working_dir();
    auto prompt_dir = working_dir.replace(0, user_home.length(), "");
    prompt_dir = "~" + prompt_dir;
    addstr(prompt_dir.c_str());
    if (m_shell_context->is_git_dir()) {
        addstr("(git) >> ");
    } else {
        addstr(" >> ");
    }
    noecho();
}

std::vector<std::shared_ptr<LakeShell::Cmd::Command>> process_input(std::string &line) {
    auto tokens = LakeShell::Tokenizer::tokenize(line);
    return LakeShell::Assembler::assemble_commands(tokens);
}