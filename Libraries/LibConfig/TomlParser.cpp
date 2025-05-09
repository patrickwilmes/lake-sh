#include "TomlParser.h"

#include <utility>
LibConfig::TomlParser::TomlParser(std::vector<Token> tokens)
    : m_tokens(std::move(tokens))
{
    m_config_store = std::make_unique<ConfigStore>();
}

std::unique_ptr<LibConfig::ConfigStore> LibConfig::TomlParser::parse()
{
    // for now we ignore sections and just provide key values
    for (size_t i = 0; i < m_tokens.size(); ++i) {
        if (m_tokens[i].type == TokenType::Key) {
            m_config_store->set(m_tokens[i].value, m_tokens[i+2].value); // ignore EQ
        }
    }
    return std::move(m_config_store);
}
