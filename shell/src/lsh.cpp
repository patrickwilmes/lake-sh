#include "lsh.hpp"
#include <iostream>
#include <string>

static const std::string EXIT_KWD = "exit";

void Lsh::run() {
    displayPrompt();
    for (std::string line; std::getline(std::cin, line);) {
        if (line == EXIT_KWD) {
            exit(EXIT_SUCCESS);
        }
        displayPrompt();
    }
}

void Lsh::displayPrompt() {
    std::cout << ">> ";
}
