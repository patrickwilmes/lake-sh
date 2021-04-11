#include "lsh.hpp"
#include "usr.hpp"
#include <iostream>
#include <string>
#include <climits>
#include <cstring>

static const std::string EXIT_KWD = "exit";

typedef struct {
    char * name;
    char * working_dir;
} render_info;

render_info * render_info_new() {
    auto *info = (render_info *) malloc(sizeof(render_info));
    info->name = (char *)malloc(sizeof(char) * lsh::usr::MAX_USERNAME_LEN);
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

void lsh::run() {
    display_prompt();
    for (std::string line; std::getline(std::cin, line);) {
        if (line == EXIT_KWD) {
            exit(EXIT_SUCCESS);
        }
        display_prompt();
    }
}

void lsh::display_prompt() {
    render_info *info = render_info_new();
    std::cout << info->name << "@" << info->working_dir << " >> ";
    render_info_free(info);
}
