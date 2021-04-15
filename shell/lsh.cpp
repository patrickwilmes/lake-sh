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

lake_shell::lake_shell() : m_win(initscr()), m_shell_context(std::make_shared<shell_context>()), m_history(history(256)), m_cmd_handler(lsh::cmd::command_handler(m_shell_context)), m_logger(m_win) {
    clear();
    cbreak();
    noecho();
    keypad(stdscr, true);

    m_shell_context->refresh();
}

lake_shell::~lake_shell() {
    endwin();
}

void lake_shell::run() {
    display_prompt();
    while (m_running) {
        char *inbuf = (char *) malloc(sizeof(char) * 2048);
        bool collecting_input = true;
        bool direct_command = false;
        int x, y;
        getyx(m_win, y, x);
        while (collecting_input) {
            int c = getch();
            switch (c) {
                case KEY_UP: {
                    std::string h_elem = m_history.next();
                    mvaddstr(y, x, h_elem.c_str());
                } break;
                case KEY_LEFT: {
                    int cx, cy;
                    getyx(m_win, cy, cx);
                    cx--;
                    move(cy, cx);
                } break;
                case KEY_RIGHT: {
                    int cx, cy;
                    getyx(m_win, cy, cx);
                    cx++;
                    move(cy, cx);
                } break;
                case KEY_ENTER:
                case ENTER_KEY:
                    mvwinstr(m_win, y, x, inbuf);
                    collecting_input = false;
                    break;
                case KEY_DC: {
                    int cx, cy;
                    getyx(m_win, cy, cx);
                    mvdelch(cy, cx);
                } break;
                case CTRL_L_KEY: {
                    collecting_input = false;
                    direct_command = true;
                    clear();
                } break;
                case KEY_BACKSPACE:
                case 127:
                    int cx, cy;
                    getyx(m_win, cy, cx);
                    mvdelch(cy, cx - 1);
                    break;
                default:
                    echochar(c);
                    break;
            }
        }
        std::string command_input(inbuf);
        trim(command_input);
        free(inbuf);
        echo();
        refresh();
        if (command_input == EXIT_KWD) {
            m_running = false;
        } else if (!direct_command) {
            if (!command_input.empty()) {
                m_logger.log_info("ah thats what i am talking about");
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
                    int cy, cx;
                    getyx(m_win, cy, cx);
                    cy++;
                    mvaddstr(cy, 0, "invalid command call: ");
                    addstr(e.what());
                } catch (command_not_found_exception &e) {
                    int cy, cx;
                    getyx(m_win, cy, cx);
                    cy++;
                    mvaddstr(cy, 0, "command not found: ");
                    addstr(e.what());
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
