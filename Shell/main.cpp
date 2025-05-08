#include "Shell.hpp"
#include <cstdlib>
#include <TomlLexer.h>

int main()
{
    // LibConfig::TomlLexer lexer("config/lake.toml");
    // auto tokens = lexer.lex();
    LakeShell::Shell shell;
    shell.run();
    return EXIT_SUCCESS;
}
