#include "usr.hpp"
#include <cstring>
#include <malloc.h>
#include <unistd.h>

constexpr int MAX_USERNAME_LEN = 32;

char *lsh::usr::get_user_name() {
    char *username = (char *) malloc(sizeof(char) * MAX_USERNAME_LEN);
    char *user = getlogin();
    strcpy(username, user);
    return username;
}
