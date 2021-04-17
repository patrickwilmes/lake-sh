#include "lsh.hpp"
#include "cmd.hpp"
#include "cmd_assembler.hpp"
#include "cmd_handler.hpp"
#include "tokenizer.hpp"
#include "utils/utils.hpp"
#include <memory>
#include <string>
#include <vector>

using namespace lsh;

static const std::string EXIT_KWD = "exit";
constexpr int CTRL_L_KEY = 12;
constexpr int ENTER_KEY = 10;

std::vector<std::shared_ptr<lsh::cmd::command>> process_input(std::string &line);

lake_shell::lake_shell() : m_shell_context(std::make_shared<shell_context>()), m_history(history(256)), m_cmd_handler(lsh::cmd::command_handler(m_shell_context)) {
    m_shell_context->refresh();
}

lake_shell::~lake_shell() {
    endwin();
}

void lake_shell::run() {
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
                } catch (invalid_command_exception &e) {
                    m_term.print_next_line("invalid command class: ");
                    m_term.print(e.what());
                } catch (command_not_found_exception &e) {
                    m_term.print_next_line("command not found: ");
                    m_term.print(e.what())
                } catch (std::runtime_error &e) {
                    m_term.print_next_line("command not found: ");
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

void lake_shell::display_prompt() {
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

std::vector<std::shared_ptr<lsh::cmd::command>> process_input(std::string &line) {
    auto tokens = lsh::tokenizer::tokenize(line);
    return lsh::assembler::assemble_commands(tokens);
}
