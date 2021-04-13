#include "lsh.hpp"
#include "cmd_assembler.hpp"
#include "cmd_handler.hpp"
#include "tokenizer.hpp"
#include "usr.hpp"
#include "utils/utils.hpp"
#include <climits>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

using namespace lsh;

static const std::string EXIT_KWD = "exit";

typedef struct {
    char *name;
    char *working_dir;
} render_info;

render_info *render_info_new();
void render_info_free(render_info *info);
std::vector<std::shared_ptr<lsh::assembler::cmd>> process_input(std::string &line);

lake_shell::lake_shell() : m_history(history(256)) {
    m_win = initscr();
    clear();
    cbreak();
    noecho();
    keypad(stdscr, true);
}

lake_shell::~lake_shell() {
    endwin();
}

void lake_shell::run() {
    while (m_running) {
        char *inbuf = (char *) malloc(sizeof(char) * 2048);
        bool collecting_input = true;
        bool direct_command = false;
        display_prompt();
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
                case 10:
                    mvwinstr(m_win, y, x, inbuf);
                    collecting_input = false;
                    clear();
                    break;
                case KEY_DC: {
                    int cx, cy;
                    getyx(m_win, cy, cx);
                    mvdelch(cy, cx);
                } break;
                case 12: {
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
        reset_shell_mode();
        if (command_input == EXIT_KWD) {
            m_running = false;
        } else if (!direct_command){
            auto cmds = process_input(command_input);
            lsh::cmd::handle_commands(cmds);
            m_history.add(command_input);
        }
        reset_prog_mode();
        noecho();
        refresh();
    }
}

void lake_shell::display_prompt() {
    render_info *info = render_info_new();
    echo();
    start_color();
    addstr(info->name);
    addstr(" @ ");
    addstr(info->working_dir);
    addstr(" >> ");
    noecho();
    render_info_free(info);
}

render_info *render_info_new() {
    auto *info = (render_info *) malloc(sizeof(render_info));
    info->name = (char *) malloc(sizeof(char) * lsh::usr::MAX_USERNAME_LEN);
    info->working_dir = (char *) malloc(sizeof(char) * PATH_MAX);
    char *username = lsh::usr::get_user_name();
    char *wd = lsh::usr::current_wd();
    char *usr = lsh::usr::usr_home_dir();
    std::string wds(wd);
    std::string usrs(usr);
    wds.replace(0, usrs.length() + 1, "");
    wds = "~/" + wds;
    strcpy(info->name, username);
    strcpy(info->working_dir, wds.c_str());
    free(username);
    free(wd);
    return info;
}

void render_info_free(render_info *info) {
    free(info->name);
    free(info->working_dir);
    free(info);
}

std::vector<std::shared_ptr<lsh::assembler::cmd>> process_input(std::string &line) {
    auto tokens = lsh::tokenizer::tokenize(line);
    return lsh::assembler::assemble_commands(tokens);
}
