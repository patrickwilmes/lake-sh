#include "ShellConfig.h"

#include "IO.h"
#include "TomlLexer.h"
#include "TomlParser.h"

LibConfig::ShellConfig::ShellConfig(const std::string& config_file)
{
    const auto config = AK::IO::read_file(config_file);
    const auto tokens = TomlLexer(config).lex();
    m_config_store = TomlParser(tokens).parse();

}

std::string LibConfig::ShellConfig::get(const std::string& key) const
{
    return m_config_store->get(key);
}
