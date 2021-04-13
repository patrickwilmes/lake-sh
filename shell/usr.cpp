#include "usr.hpp"
#include <cstring>
#include <malloc.h>
#include <unistd.h>
#include <climits>
#include <sys/types.h>
#include <pwd.h>

char *lsh::usr::get_user_name() {
    char *username = (char *) malloc(sizeof(char) * MAX_USERNAME_LEN);
    char *user = getlogin();
    strcpy(username, user);
    return username;
}

char *lsh::usr::current_wd() {
    char cwd[PATH_MAX];
    char *wd = (char *)malloc(sizeof(char) * PATH_MAX);
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        strcpy(wd, cwd);
        return wd;
    }
    return nullptr;
}
char *lsh::usr::usr_home_dir() {
    char *home = (char *) malloc(sizeof(char) * PATH_MAX);
    struct passwd *pw = getpwuid(getuid());
    strcpy(home, pw->pw_dir);
    return home;
}