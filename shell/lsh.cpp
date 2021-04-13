#include "lsh.hpp"
#include "cmd_assembler.hpp"
#include "cmd_handler.hpp"
#include "tokenizer.hpp"
#include "usr.hpp"
#include "utils/utils.hpp"
#include <climits>
#include <cstring>
#include <iostream>
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

lake_shell::lake_shell() {
    win = initscr();
    clear();
    cbreak();
    noecho();
    keypad(stdscr, true);
}

lake_shell::~lake_shell() {
    endwin();
}

void lake_shell::run() {
    while (running) {
        char *inbuf = (char *) malloc(sizeof(char) * 2048);
        bool collecting_input = true;
        display_prompt();
        int x, y;
        getyx(win, y, x);
        while (collecting_input) {
            int c = getch();
            switch (c) {
                case KEY_DOWN:
                    mvwinstr(win, y, x, inbuf);
                    collecting_input = false;
                    clear();
                    break;
                case KEY_ENTER:
                case 10:
                    mvwinstr(win, y, x, inbuf);
                    collecting_input = false;
                    clear();
                    break;
                case KEY_BACKSPACE:
                case KEY_DC:
                case 127:
                    int cx, cy;
                    getyx(win, cy, cx);
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
        move(y + 1, 0);
        reset_shell_mode();
        if (command_input == "exit") {
            running = false;
        } else {
            auto cmds = process_input(command_input);
            lsh::cmd::handle_commands(cmds);
        }
        reset_prog_mode();
        noecho();
        refresh();
    }
}

void lake_shell::display_prompt() {
    render_info *info = render_info_new();
    echo();
    curs_set(0);
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
    strcpy(info->name, username);
    strcpy(info->working_dir, wd);
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
