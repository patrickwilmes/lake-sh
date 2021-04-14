#include "cmd_assembler.hpp"
#include "cmd_handler.hpp"
#include "shell_context.hpp"
#include "tokenizer.hpp"
#include <iostream>
#include <memory>
#include <string>

void show_usage();

int main(int argc, char **argv) {
    if (argc != 2) {
        show_usage();
        return EXIT_FAILURE;
    }
    std::shared_ptr<lsh::shell_context> shell_ctx = std::make_shared<lsh::shell_context>();
    lsh::cmd::command_handler handler(shell_ctx);
    std::string command(argv[1]);
    auto tokens = lsh::tokenizer::tokenize(command);
    auto cmds = lsh::assembler::assemble_commands(tokens);
    auto response = handler.handle_commands(cmds);
    std::cout << response << '\n' << std::flush;
}

void show_usage() {
    std::cout << "Usage" << '\n'
              << std::flush;
    std::cout << "./clion_debug \"<SHELL_COMMAND>\"" << '\n'
              << std::flush;
}
