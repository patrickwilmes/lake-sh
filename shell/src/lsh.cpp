#include "lsh.hpp"
#include <iostream>
#include <string>

static const std::string EXIT_KWD = "exit";

void lsh::run() {
    displayPrompt();
    for (std::string line; std::getline(std::cin, line);) {
        if (line == EXIT_KWD) {
            exit(EXIT_SUCCESS);
        }
        displayPrompt();
    }
}

void lsh::displayPrompt() {
    std::cout << ">> ";
}
